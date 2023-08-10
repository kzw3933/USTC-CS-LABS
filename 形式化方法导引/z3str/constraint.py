from zint import Int, const_int
from zvar import Var

class Cons:
    def __init__(self, cons_name):
        self.cons_name = cons_name
        
    def print2str(self):
        raise NotImplementedError
        

class LenCons(Cons):
    def __init__(self, lhs_list, op, rhs_list, cons_name="Length"):
        super(LenCons, self).__init__(cons_name)
        self.lhs_list = LenCons.standardize(lhs_list)
        self.op = op
        self.rhs_list = LenCons.standardize(rhs_list)
    
    @staticmethod
    def standardize(var_list):
        tmp_list = []
        tmp_int = None
        for var in var_list:
            if type(var) in const_int:
                tmp_int = var if tmp_int is None else tmp_int + var
            elif type(var) is Int:
                tmp_list.append(var)
        if tmp_int is not None:
            tmp_list.append(tmp_int) 
        tmp_list = sorted(tmp_list, key=lambda x: str(x.name) if type(x) is Int else str(x)) 
        return tmp_list
        
        
    @staticmethod
    def create(lhs_list, rhs_list):
        len_lhs_list = [lhs.get_len_var() if type(lhs) is Var else len(lhs) for lhs in lhs_list]
        len_rhs_list = [rhs.get_len_var() if type(rhs) is Var else len(rhs) for rhs in rhs_list]
        return LenCons(len_lhs_list, "==", len_rhs_list)
    
    def print2str(self):
        pass
        
    
    
class Equation(Cons):
    def __init__(self, lhs_str, rhs_str, cons_name="Equation"):
        super(Equation, self).__init__(cons_name)
        self.lhs_str = lhs_str
        self.rhs_str = rhs_str
        
    def print2str(self):
        self.lhs_str.print2str()
        print(" = ", end="")
        self.rhs_str.print2str()
        print("")