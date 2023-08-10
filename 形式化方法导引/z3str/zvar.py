from zint import Int
from zexcept import EquivalentConflict

class Var:
    def __init__(self, name):
        self.name = name
        self.val = None
        self.len_var = Int(name, None)
        self.split = False
        self.children = []
        self.need_replace_use = False
        self.str_user = []
        self.equivalent_user = []
        
        
    def __eq__(self, other):
        if isinstance(other, Var):
            return self.name == other.name
        return False
        
        
    def get_len_var(self):
        return self.len_var
    
    def print2str(self):
        print("Var_"+ self.name, end=" ")
        
    def print_var(self):
        print("Var_"+ self.name+":", end=" ")
        self.print_val()
        
    def print_val(self):
        if self.val is not None:
            if type(self.val) is str:
                print(self.val, end="")
            elif type(self.val) is VarFree:
                print(self.val.get_val(), end="")
            else:
                raise TypeError("Unsupported Type: %s" % type(self.val))
        elif len(self.children) > 1:
            for child in self.children:
                if type(child) is str:
                    print(child, end="")
                elif type(child) is Var:
                    child.print_val()
        
                    
    def add_str_user(self, user):
        if user not in self.str_user:
            self.str_user.append(user)
        
    def add_equivalent_user(self, user):
        if user not in self.equivalent_user:
            self.equivalent_user.append(user)
    
    def replace_use_for_user(self):
        if self.val:
            if type(self.val) is str:
                for user in self.str_user:
                    i = 0
                    while i < len(user.var_list):
                        if user.var_list[i] == self:
                            user.var_list[i] = self.val
                        i += 1   
                    user.reduce()
                    
                for user in self.equivalent_user:
                    user.expr_list = [x for x in user.expr_list if x != self]
                    
            elif type(self.val) is VarFree:
                for user in self.str_user:
                    i = 0
                    while i < len(user.var_list):
                        if user.var_list[i] == self:
                            user.var_list[i] = self.val.get_val()
                        i += 1   
                    user.reduce()
                    
                for user in self.equivalent_user:
                    user.expr_list = [x for x in user.expr_list if x != self]
                    user.has_constant = True
                    user.constant = self.val.get_val()
                    
            else:
                raise TypeError("Unsupported Type: %s" % type(self.val))         
        else:
            for user in self.str_user:
                i = 0
                while i < len(user.var_list):
                    if user.var_list[i] == self:
                        user.var_list[i:i+1] = self.children
                        i += len(self.children)
                    else:
                        i += 1 
                
                user.rebuild_all_vars()
                
            for user in self.equivalent_user:
                user.replace(self, self.children)
                
        self.need_replace_use = False
        
    def get_len(self):
        return self.len_var.get_cur_val()
    
    def match_from_lhs(self, const_prefix):
        if not self.split:
            if len(const_prefix) == self.get_len():
                if self.val:
                    if self.val != const_prefix:
                        raise EquivalentConflict
                else:
                    self.val = const_prefix
                    self.need_replace_use = True
                    return None
            else:
                child = Var(self.name + "_1")
                child.get_len_var().set_val(self.get_len()-len(const_prefix))
                self.children = [const_prefix, child]
                self.need_replace_use = True
                return child
        else:
            surplus_index = 0
            surplus_fragment = None
            for i, child in enumerate(self.children):
                if type(child) is str:
                    if len(child) == len(const_prefix):
                        if child == const_prefix:
                            surplus_index = i
                            break
                        else:
                            raise EquivalentConflict
                    elif len(child) > len(const_prefix):
                        if child.startswith(const_prefix):
                            surplus_index = i
                            surplus_fragment = child[len(const_prefix):]
                            break
                        else:
                            raise EquivalentConflict
                    else:
                        if const_prefix.startswith(child):
                            const_prefix = const_prefix[len(child):]
                        else:
                            raise EquivalentConflict
                elif type(child) is Var:
                    var_len = child.get_len()
                    if var_len >= len(const_prefix):
                        surplus_index = i
                        surplus_fragment = child.match_from_lhs(const_prefix)
                    else:
                        child.match_from_lhs(const_prefix[0:var_len])
                        const_prefix = const_prefix[var_len:]
            
            for i, child in self.children:
                if child.split:
                    self.children[i:i+1] = child.children 
                    self.need_replace_use = True
                    break 
            
            fragments = []
            if surplus_fragment is not None and type(surplus_fragment) is list and len(surplus_fragment) > 0:
                fragments.extend(surplus_fragment)
            elif surplus_fragment is not None:
                fragments.append(surplus_fragment)
                
            for i in range(surplus_index+1, len(self.children)):
                fragments.append(self.children[i])
            
            if len(fragments) == 1:
                fragments[0]
            elif len(fragments) == 0:
                return None
            else:
                return fragments
   
                        
    
    def match_from_rhs(self, const_suffix):
        if not self.split:
            if len(const_suffix) == self.get_len():
                if self.val:
                    if self.val != const_suffix:
                        raise EquivalentConflict
                else:
                    self.val = const_suffix
                    self.need_replace_use = True
                    return None 
            else:
                child = Var(self.name + "_1")
                child.get_len_var().set_val(self.get_len()-len(const_suffix))
                self.children = [child, const_suffix]
                self.need_replace_use = True
                return child
        else:
            surplus_index = 0
            surplus_fragment = None
            for i, child in enumerate(reversed(self.children)):
                if type(child) is str:
                    if len(child) == len(const_suffix):
                        if child == const_suffix:
                            surplus_index = i
                            break
                        else:
                            raise EquivalentConflict
                    elif len(child) > len(const_suffix):
                        if child.endswith(const_suffix):
                            surplus_index = i
                            surplus_fragment = child[0:-len(const_suffix)]
                            break
                        else:
                            raise EquivalentConflict
                    else:
                        if const_suffix.endswith(child):
                            const_suffix = const_suffix[:-len(child)]
                        else:
                            raise EquivalentConflict
                elif type(child) is Var:
                    var_len = child.get_len()
                    if var_len >= len(const_suffix):
                        surplus_index = i
                        surplus_fragment = child.match_from_rhs(const_suffix)
                    else:
                        child.match_from_rhs(const_suffix[-var_len:])
                        const_suffix = const_suffix[0:-var_len]
                        
            for i, child in self.children:
                if child.split:
                    self.children[i:i+1] = child.children
                    self.need_replace_use = True
                    break
            
            fragments = []     
            for i in range(0, len(self.children)-1-surplus_index):
                fragments.append(self.children[i])
    
            if surplus_fragment is not None and type(surplus_fragment) is list and len(surplus_fragment) > 0:
                fragments.extend(surplus_fragment)
            elif surplus_fragment is not None:
                fragments.append(surplus_fragment)
                
            if len(fragments) == 1:
                fragments[0]
            elif len(fragments) == 0:
                return None
            else:
                return fragments
        
class VarFree(Var):
    count = 1
    def __init__(self, var):
        super().__init__("v"+str(VarFree.count))
        VarFree.count += 1
        if type(var) is Var:
            self.len_var = var.len_var
        else:
            raise TypeError("Unsupported Type: %s" % type(var))
        
    def get_val(self):
        if not self.val:
            self.val = "a"*self.get_len()
            
        return self.val
