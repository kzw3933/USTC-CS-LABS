from zvar import Var, VarFree
from zstr import Str
from zexcept import EquivalentConflict
class Equivalent:
    def __init__(self, equivalent_name):
        self.equivalent_name = equivalent_name
        
    def append(self, expr):
        raise NotImplementedError
    
class StrEquivalent(Equivalent):
    count = 1
    def __init__(self, equivalent_name = "Str"):
        super().__init__(equivalent_name)
        self.name = "e"+str(StrEquivalent.count)
        StrEquivalent.count += 1
        self.expr_list = []
        self.all_vars = []
        self.has_constant = False
        self.constant = None
        
    def __eq__(self, other):
        if isinstance(other, StrEquivalent):
            return self.name == other.name
        return False
    
    def replace(self, old_var, new_var):
        index = 0
        for i, expr in enumerate(self.expr_list):
            if expr == old_var:
                index = i
                
        if len(new_var) == 1:
            self.expr_list[index] = new_var
        else:
            new_str = Str()
            for var in new_var:
                new_str.append(var)
            self.expr_list[index] = new_str
             
    
    @staticmethod
    def merge(lhs, rhs):
        for expr in rhs.expr_list:
            lhs.append(expr)
            
    def rebuild_all_vars(self):
        all_vars = []
        for expr in self.expr_list:
            if type(expr) is Var and expr not in all_vars:
                all_vars.append(expr)
            elif type(expr) is Str:
                vars = expr.get_all_vars()
                if len(vars) > 0:
                    for var in vars:
                        if var not in all_vars:
                            all_vars.append(var)
                
        self.all_vars = all_vars
        
    def append(self, expr):
        if type(expr) is str:
            if not self.has_constant:
                self.has_constant = True
                self.constant = expr
            else:
                if self.constant != expr:
                    raise EquivalentConflict
        elif type(expr) is Str:
            self.expr_list.append(expr)
        elif type(expr) is Var:
            expr.add_equivalent_user(self)
            self.expr_list.append(expr)
            
        else:
            raise TypeError("Unsupported Type: %s" % type(expr))

            
    def find(self, expr):
        if expr in self.expr_list:
            return True
        return False
    
    def get_constant(self):
        return self.constant  
    
    def get_max_const_prefix(self):
        if self.has_constant:
            return self.constant
        else:
            max_const_prefix = None
            for expr in self.expr_list:                    
                if max_const_prefix is None:
                    if type(expr) is Str and type(expr.var_list[0]) is str:
                        max_const_prefix = expr.var_list[0]
                else:
                    if type(expr) is Str and type(expr.var_list[0]) is str and len(expr.var_list[0]) > len(max_const_prefix):
                        max_const_prefix = expr.var_list[0]
                        
            return max_const_prefix
            
        
    def get_max_const_suffix(self):
        if self.has_constant:
            return None
        else:
            max_const_suffix = None
            for expr in self.expr_list:
                if max_const_suffix is None:
                    if type(expr) is Str and type(expr.var_list[-1]) is str:
                        max_const_suffix = expr.var_list[-1]
                else:
                    if type(expr) is Str and type(expr.var_list[-1]) is str and len(expr.var_list[-1]) > len(max_const_suffix):
                        max_const_suffix = expr.var_list[-1]
                        
            return max_const_suffix
        
    def get_len(self):
        if type(self.expr_list[0]) is str:
            return len(self.expr_list[0])
        elif type(self.expr_list[0]) is Var:
            return self.expr_list[0].get_len()
        elif type(self.expr_list[0]) is Str:
            return self.expr_list[0].get_len()
        else:
            raise TypeError("Unsupported Type: %s" % type(self.expr_list[0]))
        
    def replace_use_of_var(self):
        for var in self.all_vars:
            if var.need_replace_use:
                var.replace_use_for_user()
                
    def assign_free_var(self):
        self.rebuild_all_vars()
        self.all_vars[0].val = VarFree(self.all_vars[0])
        self.all_vars[0].need_replace_use = True
        self.all_vars[0].replace_use_for_user()
    
    def reduce(self):
        self.rebuild_all_vars()
        max_const_prefix = self.get_max_const_prefix()
        max_const_suffix = self.get_max_const_suffix()
        
        if max_const_prefix:
            new_equivalent = StrEquivalent()
            for expr in self.expr_list:
                if type(expr) is str:
                    if expr.startswith(max_const_prefix):
                        new_str = expr[len(max_const_suffix):]
                    else:
                        raise EquivalentConflict
                elif type(expr) is Var:
                    new_str = expr.match_from_lhs(max_const_prefix)
                elif type(expr) is Str:
                    new_str = expr.match_from_lhs(max_const_prefix)
                else:
                    raise TypeError("Unsupported Type: %s" % type(expr))
                
                if new_str:
                    new_equivalent.append(new_str)
                
                self.replace_use_of_var()
            
            return new_equivalent if len(new_equivalent.expr_list) > 0 else None, True
                
        elif max_const_suffix:
            new_equivalent = StrEquivalent()
            for expr in self.expr_list:
                if type(expr) is str:
                    if expr.endswith(max_const_suffix):
                        new_str = expr[:-len(max_const_suffix)]
                    else:
                        raise EquivalentConflict
                elif type(expr) is Var:
                    new_str = expr.match_from_rhs(max_const_suffix)
                elif type(expr) is Str:
                    new_str = expr.match_from_rhs(max_const_suffix)
                else:
                    raise TypeError("Unsupported Type: %s" % type(expr))
                
                if new_str:
                    new_equivalent.append(new_str)
                    
                self.replace_use_of_var()
                
            return new_equivalent if len(new_equivalent.expr_list) > 0 else None, True
        
        else:
            return self, False
            
            
                
                
                