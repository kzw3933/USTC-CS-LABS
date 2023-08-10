from zvar import Var 
from zexcept import EquivalentConflict

class Str:
    def __init__(self):
        self.var_list = []
        self.all_vars = []
        self.equivalent = None
        
    def __eq__(self, other):
        if isinstance(other, Str):
            return self.var_list == other.var_list
        return False
    
    def rebuild_all_vars(self):
        all_vars = []
        for var in self.var_list:
            if type(var) is Var and var not in all_vars:
                all_vars.append(var)
                var.add_str_user(self)
        self.all_vars = all_vars
        
    def append(self, var):
        if type(var) is str: 
            if len(self.var_list) > 0 and type(self.var_list[-1]) is str:
                self.var_list[-1] += var
            else:
                self.var_list.append(var)
        elif type(var) is Var:
            var.add_str_user(self)
            self.var_list.append(var)
            self.all_vars.append(var)
        elif type(var) is Str:
            if len(var.var_list) > 0 and type(var.var_list[0]) is str and  len(self.var_list) > 0 and type(self.var_list[-1]) is str:
                self.var_list[-1] += var.var_list[0]
                if len(var.var_list) > 1:
                    for i in var.var_list[1:]:
                        self.append(i)
            elif len(var.var_list) > 0:
                for i in var.var_list:
                    self.append(i)
            vars = var.get_all_vars()
            if len(vars) > 0:
                self.all_vars.extend(vars)
        else:
            raise TypeError("Unsupported Type: %s" % type(var))
        
    def reduce(self):
        tmp_var_list = []
        tmp_all_vars = []
        for var in self.var_list:
            if type(var) is str: 
                if len(tmp_var_list) > 0 and type(tmp_var_list[-1]) is str:
                    tmp_var_list[-1] += var
                else:
                    tmp_var_list.append(var)
            elif type(var) is Var:
                tmp_var_list.append(var)
                tmp_all_vars.append(var)
        self.var_list = tmp_var_list
        self.all_vars = tmp_all_vars
        
    def get_all_vars(self):
        return self.all_vars
        
    def get_len(self):
        total_len = 0
        for var in self.var_list:
            if type(var) is str:
                total_len += len(var)
            elif type(var) is Var:
                total_len += var.get_len()
            else:
                raise TypeError("Unsupported Type: %s" % type(var))
            
        return total_len
        
    def print2str(self):
        for var in self.var_list:
            if type(var) is str:
                print(var, end=" ")
            elif type(var) is Var:
                var.print2str()
            else:
                raise TypeError("Unsupported Type: %s" % type(var))
            
    def match_from_lhs(self, const_prefix):
        surplus_index = 0
        surplus_fragment = None
        
        for i, var in enumerate(self.var_list):
            if type(var) is str:
                str_len = len(var)
                if str_len == len(const_prefix):
                    if const_prefix == var:
                        surplus_index = i
                        break
                    else:
                        raise EquivalentConflict
                elif str_len > len(const_prefix):
                    if var.startswith(const_prefix):
                        surplus_index = i
                        surplus_fragment = var[len(const_prefix):]
                        break
                    else:
                        raise EquivalentConflict
                else:
                    if const_prefix.startswith(var):
                        const_prefix = const_prefix[str_len:]
                    else:
                        raise EquivalentConflict
            elif type(var) is Var:
                var_len = var.get_len()
                if var_len >= len(const_prefix):
                    surplus_index = i
                    surplus_fragment = var.match_from_lhs(const_prefix)
                    break
                else:
                    var.match_from_lhs(const_prefix[0:var_len])
                    const_prefix = const_prefix[var_len:]
                    
        fragments = []
        if type(surplus_fragment) is list and len(surplus_fragment) > 0:
            fragments.extend(surplus_fragment)
        elif surplus_fragment:
            fragments.append(surplus_fragment)
        for i in range(surplus_index+1, len(self.var_list)):
            fragments.append(self.var_list[i])
        
        if len(fragments) == 1:
            return fragments[0]  
        elif len(fragments) == 0:
            return None
        else:
            new_str = Str()
            for i in fragments:
                new_str.append(i)                
            return new_str
            
    
    def match_from_rhs(self, const_suffix):
        surplus_index = 0
        surplus_fragment = None            
        if const_suffix:
            for i, var in enumerate(reversed(self.var_list)):
                if type(var) is str:
                    str_len = len(var)
                    if str_len == len(const_suffix):
                        if const_suffix == var:
                            surplus_index = i
                            break
                        else:
                            raise EquivalentConflict
                    elif str_len > len(const_suffix):
                        if var.endswith(const_suffix):
                            surplus_index = i
                            surplus_fragment = var[:-len(const_suffix)]
                            break
                        else:
                            raise EquivalentConflict
                    else:
                        if const_suffix.endswith(var):
                            const_suffix = const_suffix[:-str_len]
                        else:
                            raise EquivalentConflict
                elif type(var) is Var:
                    var_len = var.get_len()
                    if var_len >= len(const_suffix):
                        surplus_index = i
                        surplus_fragment = var.match_from_rhs(const_suffix)
                        break
                    else:
                        var.match_from_rhs(const_suffix[-var_len:])
                        const_suffix = const_suffix[0:-var_len]
                        
        fragments = []
        
        for i in range(0, len(self.var_list)-1-surplus_index):
            fragments.append(self.var_list[i])
        
        if type(surplus_fragment) is list and len(surplus_fragment) > 0:
            fragments.extend(surplus_fragment)
        elif surplus_fragment:
            fragments.append(surplus_fragment)
            
        if len(fragments) == 1:
            return fragments[0]
        elif len(fragments) == 0:
            return None
        else:
            new_str = Str()
            for i in fragments:
                new_str.append(i)
                
            return new_str
                        

    
                
            
        
