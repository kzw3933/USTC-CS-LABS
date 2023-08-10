import numpy as np
import math

from zint import Int, IntFree, IntExpr, IntOp, const_int
from zvar import Var
from equivalent import StrEquivalent
from zexcept import SystemOfEquationsConflict, OverBound, Divisible

class Solver:
    def __init__(self, solver_name):
        self.solver_name = solver_name
        
    def solve(self):
        raise NotImplementedError
    
class LenSolver(Solver):
    def __init__(self, len_bound, solver_name="Length Solver"):
        super(LenSolver, self).__init__(solver_name)
        self.cons_list = []
        self.all_vars = None
        self.cofficient_matrix = None
        self.val_vector = None
        self.len_bound = len_bound
        self.free_vars = []
        
    def init_cons_list(self, cons_list):
        self.cons_list = cons_list
        
    def append_cons(self, cons):
        self.cons_list.append(cons)
    
    def gen_all_vars(self):
        all_vars = []
        for cons in self.cons_list:
            for lhs in cons.lhs_list:
                if type(lhs) is Int and lhs not in all_vars:
                    all_vars.append(lhs)
            for rhs in cons.rhs_list:
                if type(rhs) is Int and rhs not in all_vars:
                    all_vars.append(rhs)
        
        self.all_vars = sorted(all_vars, key=lambda x: str(x.name))         
    
    def gen_coefficient_matrix(self):
        M = len(self.cons_list)
        N = len(self.all_vars)
        cofficient_matrix = np.zeros((M, N), dtype=int)
        for i, cons in enumerate(self.cons_list):
            for j, var in enumerate(self.all_vars):
                for k in cons.lhs_list:
                    if k == var:
                        cofficient_matrix[i][j] += 1
                for k in cons.rhs_list:
                    if k == var:
                        cofficient_matrix[i][j] -= 1                   
                    
        self.cofficient_matrix = cofficient_matrix          
        
        
    def gen_val_vector(self):
        M = len(self.cons_list)
        val_vector = np.zeros((M, 1), dtype=int)
        for i, cons in enumerate(self.cons_list):
            if type(cons.lhs_list[0]) in const_int and type(cons.rhs_list[0]) in const_int:
                val_vector[i] = cons.rhs_list[0] - cons.lhs_list[0]
            elif type(cons.lhs_list[0]) in const_int:
                val_vector[i] = - cons.lhs_list[0]
            elif type(cons.rhs_list[0]) in const_int:
                val_vector[i] = cons.rhs_list[0]
                
        self.val_vector = val_vector
        
    def row_echelon_form(self):
        Ab = np.concatenate((self.cofficient_matrix, self.val_vector), axis=1, dtype=int)
        M = len(self.cons_list)
        N = len(self.all_vars)
        
        for i in range(N):
            max_row = i
            if i > M - 1:
                break
            for j in range(i+1, M):
                if abs(Ab[j, i]) > abs(Ab[max_row, i]):
                    max_row = j
            Ab[[i, max_row], :] = Ab[[max_row, i], :]
            if Ab[i, i] == 0:
                continue
            for j in range(i+1, M):
                if Ab[j, i] == 0:
                    continue
                else:
                    lcm = Ab[i, i] * Ab[j, i] // math.gcd(Ab[i, i], Ab[j, i])
                    Ab[j, i:] = (lcm / Ab[j, i]) * Ab[j, i:] - (lcm / Ab[i, i]) * Ab[i, i:]
                    
        for i in range(M):
            gcd = np.gcd.reduce(Ab[i][Ab[i] != 0])
            Ab[i] = Ab[i] if gcd == 0 else Ab[i] / gcd
            
        unique_Ab = []
        for i in Ab:
            occur = False
            for j in unique_Ab:
                if np.array_equal(i, j):
                    occur = True
                    break
            if not occur:                    
                unique_Ab.append(i)
        unique_Ab = np.array(unique_Ab)
        sorted_Ab = sorted(unique_Ab, key=lambda x: (x != 0).tolist(), reverse=True)
        sorted_Ab = np.array(sorted_Ab)
        self.cofficient_matrix = sorted_Ab[:,:-1]
        self.val_vector = sorted_Ab[:,-1]
        
        
    def get_model(self):
        free_vars = []
        N = len(self.cofficient_matrix)
        for i, row in enumerate(reversed(self.cofficient_matrix)):
            var_index = None
            for j in range(len(row)):
                if row[j] != 0:
                    var_index = j
                    break
            if var_index is None:
                continue
            if self.all_vars[var_index].val is None:
                int_expr = IntExpr() 
                int_expr.add_var_op((None, self.val_vector[N-1-i]))
                for j in range(var_index+1, len(self.all_vars)):
                    if self.cofficient_matrix[N-1-i][j] != 0:
                        if self.all_vars[j].val is None:
                            tmp_val = IntFree(self.len_bound)
                            self.all_vars[j].set_val(tmp_val)
                            tmp_val.bind_int_var(self.all_vars[j])
                            free_vars.append(tmp_val)
                        int_expr.add_var_op((IntOp("mul", -self.cofficient_matrix[N-1-i][j]), self.all_vars[j].val))
                
                int_expr2 = IntExpr()
                int_expr2.add_var_op((IntOp("div", self.cofficient_matrix[N-1-i][var_index]), int_expr))
                self.all_vars[var_index].val = int_expr2  
        self.free_vars = sorted(free_vars, key=lambda x: str(x.name))
    
    def check_assignments(self):
        for var in self.all_vars:
            if var.get_cur_val() > 0:
                continue
            else:
                    return False
        return True
    
    def init_all_assignments(self):
        for var in self.free_vars:
            var.reset()
            var.get_val()
        try:
            for var in self.all_vars:
                var.get_val()
        except Divisible:
            return False
            
        return self.check_assignments()
            
    def check_echelon_form(self):
        for i, irow in enumerate(self.cofficient_matrix):
            if np.all(irow == 0) and self.val_vector[i] != 0:
                raise SystemOfEquationsConflict
            for j in range(i+1, len(self.cofficient_matrix)):
                if np.array_equal(self.cofficient_matrix[i], self.cofficient_matrix[j]):
                    if self.val_vector[i] != self.val_vector[j]:
                        raise SystemOfEquationsConflict
    
    def print2str(self):
        for var in self.all_vars:
            var.print2str()  
            
    def regen_all_assignments(self):
        index = len(self.free_vars)-1
        while True:
            if not self.free_vars[index].reach_bound():
                break
            else:
                index -= 1
                if index < 0:
                    break
                
        if index < 0:
            raise OverBound
        else:
            for i in range(len(self.free_vars)-1, index, -1):
                self.free_vars[i].init()
                self.free_vars[i].get_val()
            self.free_vars[index].reset()
            self.free_vars[index].get_val()
            
        try:
            for var in self.all_vars:
                var.get_val()
        except Divisible:
            return False
            
        return self.check_assignments()
            
                      
        
    def solve(self):
        self.gen_all_vars()
        self.gen_val_vector()
        self.gen_coefficient_matrix()
        self.row_echelon_form()
        self.check_echelon_form()
        self.get_model()
        if self.init_all_assignments():
            return
        else:
            while True:
                res = self.regen_all_assignments()
                if res:
                    break
        

class StrEquationSolver(Solver):
    def __init__(self, solver_name="String Equation Solver"):
        super(StrEquationSolver, self).__init__(solver_name)
        self.cons_list = []
        self.all_vars = []
        self.all_equivalents = []
        self.new_all_equivalents = []
        
    def init_cons_list(self, cons_list):
        self.cons_list = cons_list
        
    def append_cons(self, cons):
        self.cons_list.append(cons)
        
    def gen_all_vars(self):
        all_vars = []
        for cons in self.cons_list:
            for lhs in cons.lhs_str.var_list:
                if type(lhs) is Var and lhs not in all_vars:
                    all_vars.append(lhs)
            for rhs in cons.rhs_str.var_list:
                if type(rhs) is Var and rhs not in all_vars:
                    all_vars.append(rhs)
        
        self.all_vars = sorted(all_vars, key=lambda x: str(x.name)) 
        
    def find_equivalent(self, expr):
        for equivalent in self.all_equivalents:
            if equivalent.find(expr):
                return equivalent
        return None
        
    def init_all_equivalents(self):
        all_equivalents = []
        for cons in self.cons_list:
            tmp_equivalent = None
            if cons.lhs_str.equivalent is None:
                tmp_equivalent = StrEquivalent()
                all_equivalents.append(tmp_equivalent)
                tmp_equivalent.append(cons.lhs_str)
            if cons.rhs_str.equivalent is None:
                tmp_equivalent.append(cons.rhs_str)
            else:
                if cons.rhs_str.equivalent != tmp_equivalent:
                    StrEquivalent.merge(cons.rhs_str.equivalent, tmp_equivalent)
                    all_equivalents.remove(tmp_equivalent)
                
        self.all_equivalents = all_equivalents
        
    def reduce(self):
        new_all_equivalents = []
        changed = False
        for equivalent in self.all_equivalents:
            new_equivalent, res = equivalent.reduce()
            if not changed and res:
                changed = True
            if new_equivalent:
                new_all_equivalents.append(new_equivalent)
            
        return new_all_equivalents, changed  
    
    def check_success(self):
        for equivalent in self.all_equivalents:
            if not equivalent.has_constant:
                return False
            
        return True
    
    def print2str(self):
        for var in self.all_vars:
            var.print_var()
            print("")
                
        
    def solve(self):
        self.gen_all_vars()
        self.init_all_equivalents()
        while True:
            while True:
                new_all_equivalents, changed = self.reduce()
                if not changed:
                    break
                self.all_equivalents = new_all_equivalents
            if self.check_success():
                return
            self.all_equivalents[0].assign_free_var()
            
    
