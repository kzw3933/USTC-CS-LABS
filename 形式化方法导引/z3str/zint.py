import math
import numpy as np

from zexcept import Divisible

const_int = (int, np.int64, np.int32)

class IntOp:
    def __init__(self, name, factor):
        self.name = name
        self.factor = factor
        
    def operate(self, expr):
        if self.name == "mul":
            return int(self.factor * expr.get_val())
        elif self.name == "div":
            if expr.get_val() % self.factor != 0:
                raise Divisible
            return int(expr.get_val() / self.factor)

class Int:
    def __init__(self, name, val=None):
        self.name = name
        self.val = val
        self.cur_val = None
        
    def __eq__(self, other):
        if isinstance(other, Int):
            return self.name == other.name
        return False
        
    def set_val(self, val):
        if type(val) in const_int:
            self.cur_val = val
        self.val = val
    
    def get_cur_val(self):
        return self.cur_val
        
    def reset(self):
        self.cur_val = None
        
    def get_val(self):
        self.cur_val = self.compute_val()
        
    def compute_val(self):
        if type(self.val) in const_int:
            return self.val
        else:
            return self.val.get_val()
    
    def gen_factor(self):
        if self.val is None:
            return []
        else:
            factors = []
            for i in range(1, int(math.sqrt(self.val))+1):
                if self.val % i == 0:
                    factors.append(i)
                    if self.val // i != i:
                        factors.append(self.val // i)
            return sorted(factors)
        
    def gen_addend(self):
        if self.val is None:
            return []
        else:
            addends = []
            for i in range(self.val+1):
                addends.append(i)
            return addends
        
    def print2str(self):
        print(self.name + ": "+ str(self.cur_val))
        
class IntFree(Int):
    count = 1
    def __init__(self, bound):
        super().__init__("i"+str(IntFree.count), 1)
        self.bound = bound
        self.int_var = None
        IntFree.count += 1
        
    def init(self):
        self.val = 1
        self.cur_val = None
        
    def reset(self):
        self.cur_val = None
        self.int_var.cur_val = None
        
    def bind_int_var(self, int_var):
        self.int_var = int_var
        
    def get_val(self):
        if self.cur_val is None:
            self.cur_val = self.val
            self.val += 1
        return self.cur_val
    
    def reach_bound(self):
        return self.cur_val == self.bound
        
class IntExpr:
    def __init__(self):
        self.vars = []
        self.ops = []
        
    def add_var_op(self, var_op):
        op, var = var_op
        self.vars.append(var)
        self.ops.append(op)
    
    def get_val(self):
        ret = 0
        for i, var in enumerate(self.vars):
            if self.ops[i] is None:
                if type(var) in const_int:
                    ret += var
                elif type(var) is Int:
                    ret += var.compute_val()
                else:
                    ret += var.get_val()
                
            else:
                ret += self.ops[i].operate(var)
                
        return ret    
    
    
    