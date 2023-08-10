import copy

from constraint import LenCons
from solver import LenSolver, StrEquationSolver
from zvar import Var
from zexcept import EquivalentConflict, SystemOfEquationsConflict, OverBound
from zint import const_int

class Driver:
    def __init__(self, len_bound=None):
        self.all_vars = []
        self.all_equivalence_class = []
        self.equation_constraints = []
        self.length_constraints = []
        self.len_bound = len_bound
        self.len_solver = None
        
    def set_len_bound(self, len_bound):
        if type(len_bound) in const_int:
            self.len_bound = len_bound
        else:
            raise TypeError("Unsupported Type: %s" % type(len_bound))
        
    
    def append_constraints(self, constraints):
        for cons in constraints:
            self.append_constraint(cons)
    
    def append_constraint(self, constraint):
        if constraint.cons_name == "Equation":
            self.equation_constraints.append(constraint)
        elif constraint.cons_name == "Length":
            self.length_constraints.append(constraint)
            
    def solve(self):
        self.gen_all_vars_from_equations()
        self.gen_length_constraints_from_equations()
        self.len_solver = LenSolver(self.len_bound)
        self.len_solver.init_cons_list(self.length_constraints)       
        for cons in self.equation_constraints:
            cons.print2str()
        print("")
        try:
            self.len_solver.solve()
        except SystemOfEquationsConflict as e:
            print(e.message)
            exit(-1)
        while True:
            try:
                backup = copy.deepcopy(self)
                str_solver = StrEquationSolver()
                str_solver.init_cons_list(self.equation_constraints)
                str_solver.solve()
                print("//"*50)
                self.len_solver.print2str()
                str_solver.print2str()
                print("//"*50)
                break
            except EquivalentConflict as e:
                print("//"*50)
                self.len_solver.print2str()
                print(e.message)
                print("//"*50)
                print("")
                try:
                    res = False
                    while not res:
                        res = backup.len_solver.regen_all_assignments()
                    self = backup
                except OverBound as e:
                    print("//"*50)
                    self.len_solver.print2str()
                    print(e.message)
                    print("//"*50)
                    print("")
                    exit(-1)   
     
        
    def gen_all_vars_from_equations(self):
        for equation in self.equation_constraints:
            for var in equation.lhs_str.var_list:
                if var not in self.all_vars and type(var) is Var:
                    self.all_vars.append(var)
            for var in equation.rhs_str.var_list:
                if var not in self.all_vars and type(var) is Var:
                    self.all_vars.append(var)    
        
    def gen_length_constraints_from_equations(self):
        for equation in self.equation_constraints:
            self.length_constraints.append(LenCons.create(equation.lhs_str.var_list, equation.rhs_str.var_list))
    
    def get_model():
        pass
    
    
    
    
    
    