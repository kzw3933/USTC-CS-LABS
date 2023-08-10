from parse import parser
from robdd import ROBDD

class TSM:
    def __init__(self, states, attributes_dict, transitions, verify):
        """
        A class for transition system model
        
        states: a tuple of states that identified with name
        attributes_dict: a map of state and its attributes
        transitions: a set of transition tuple such that ('s1', 's2') to express there is a transition from s1 to s2
        verify: to be verified ctl in the form of a tuple such that ('s0', 'AFq) 
         
        Example:
        >>> states = ('s0', 's1', 's2', 's3')
        >>> attributes_dict = {'s3': ("q")}
        >>> transitions = (('s0', 's0'), ('s0', 's1'), ('s0', 's2'), ('s1', 's3'), ('s3', 's0'), ('s2', 's1'), ('s2', 's3'))
        >>> verify = ('s0', 'AFp')
        >>> tsm = TSM(states, attributes_dict, transitions, verify)  
        """
        self.state2enc, self.enc2state = self.__encode_states(states)
        self.attributes_dict = attributes_dict
        self.transitions_set = self.__build_transition(transitions)
        self.verify_st, self.verify_ctl = verify
        self.robdd = ROBDD()
        self.parser = parser
        self.transitions_robdd = self.__compute_transitions()
        
        
    def __build_transition(self, transitions):
        return set([self.state2enc[trans[0]]+self.state2enc[trans[1]] for trans in transitions])
    
    def __encode_states(self, states):
        state2enc, enc2state = dict(), dict()
        num = len(states)
        bitwidth = 1
        while 2**bitwidth < num:
            bitwidth = bitwidth + 1
        for i, st in enumerate(states):
            encode = self.__num2str(i, bitwidth) 
            state2enc[st] = encode
            enc2state[encode] = st
        
        return state2enc, enc2state
            
    def __num2str(self, num, bitwidth):
        bits = ["0"]*bitwidth
        for i in range(bitwidth):
            bits[i] = str((num >> i) & 1)
        return ''.join(bits[::-1])
    
    def __get_boolean_expr(self, elements, bi=False):
        or_list = []
        for element in elements:
            if bi:
                element_len_bisect = int(len(element) / 2)
                and_list1 = ['p'+str(i) if element[i] == '1' else '~p'+str(i) for i in range(element_len_bisect)]  
                and_list2 = ['q'+str(i) if element[element_len_bisect+i] == '1' else '~q'+str(i) for i in range(element_len_bisect)]
                
                or_list.append(' & '.join(and_list1+and_list2))
            else:
                and_list = ['p'+str(i) if element[i] == '1' else '~p'+str(i) for i in range(len(element))]
                or_list.append(' & '.join(and_list))
        return ' | '.join(or_list)
    
    def __rename_bdd_tree(self, bdd_tree):
        if bdd_tree[0] == 'VARIABLE':
            return ('VARIABLE', bdd_tree[1].replace('p','q'), self.__rename_bdd_tree(bdd_tree[2]), self.__rename_bdd_tree(bdd_tree[3]))
        else:
            return bdd_tree
        
    def __pruning_bdd_tree(self, bdd_tree):
        if bdd_tree[0] == 'VARIABLE' and bdd_tree[1][0] == 'q':
            return ('CONSTANT', 1)
        elif bdd_tree[0] == 'VARIABLE' and bdd_tree[1][0] == 'p':
            return ('VARIABLE', bdd_tree[1], self.__pruning_bdd_tree(bdd_tree[2]), self.__pruning_bdd_tree(bdd_tree[3]))
        else:
            return bdd_tree
    
    def __compute_transitions(self):
        return self.robdd.build(self.parser.parse(self.__get_boolean_expr(self.transitions_set, bi=True)), rebuild=True)
    
    def __compute_var(self, var):
        or_list = []
        for state, attributes in self.attributes_dict.items():
            if var in attributes:
                or_list.append(state)
        return self.robdd.build(self.parser.parse(self.__get_boolean_expr([self.state2enc[st] for st in or_list])), rebuild=True)
    
    def __compute_eg(self, expr):
        expr_bdd_tree = self.__compute_expr(expr)
        old_t = None
        new_t = expr_bdd_tree
        while old_t == None or new_t != old_t:
            old_t = new_t
            rename_bdd_tree = self.__rename_bdd_tree(old_t)
            bdd_tree = self.robdd.build(('AND', self.transitions_robdd, rename_bdd_tree))
            new_t = self.robdd.build(('AND', self.__pruning_bdd_tree(bdd_tree), expr_bdd_tree))
        return new_t
        
    
    def __compute_ex(self, expr):
        expr_bdd_tree = self.__compute_expr(expr)
        rename_bdd_tree = self.__rename_bdd_tree(expr_bdd_tree)
        bdd_tree = self.robdd.build(('AND', self.transitions_robdd, rename_bdd_tree))
        return self.robdd.build(self.__pruning_bdd_tree(bdd_tree)) 
    
    def __compute_eu(self, expr1, expr2):
        expr1_bdd_tree = self.__compute_expr(expr1)
        expr2_bdd_tree = self.__compute_expr(expr2)
        old_t = None
        new_t = expr2_bdd_tree
        while old_t == None or new_t != old_t:
            old_t = new_t
            rename_bdd_tree = self.__rename_bdd_tree(old_t)
            bdd_tree = self.robdd.build(('AND', self.transitions_robdd, rename_bdd_tree))
            new_t = self.robdd.build(('AND', self.__pruning_bdd_tree(bdd_tree), expr1_bdd_tree))
        return new_t
    
    def __compute_not(self, expr):
        return self.robdd.build(('IMPLIES', self.__compute_expr(expr), ('CONSTANT', 0)))
        
    def __compute_and(self, expr1, expr2):
        return self.robdd.build(('AND', self.__compute_expr(expr1), self.__compute_expr(expr2)))
        
    def __compute_or(self, expr1, expr2):
        return self.robdd.build(('OR', self.__compute_expr(expr1), self.__compute_expr(expr2)))
    
    def __compute_implies(self, expr1, expr2):
        return self.robdd.build(('IMPLIES', self.__compute_expr(expr1), self.__compute_expr(expr2)))
        
    def __compute_iff(self, expr1, expr2):
        return self.robdd.build(('IFF', self.__compute_expr(expr1), self.__compute_expr(expr2)))
    
    def __compute_expr(self, expr):
        if expr[0] == 'EG':
            return self.__compute_eg(expr[1])
        elif expr[0] == 'EX':
            return self.__compute_ex(expr[1])
        elif expr[0] == 'EU':
            return self.__compute_eu(expr[1], expr[2])
        elif expr[0] == 'NOT':
            return self.__compute_not(expr[1])
        elif expr[0] == 'AND':
            return self.__compute_and(expr[1], expr[2])
        elif expr[0] == 'OR':
            return self.__compute_or(expr[1], expr[2])
        elif expr[0] == 'IMPLIES':
            return self.__compute_implies(expr[1], expr[2])
        elif expr[0] == 'IFF':
            return self.__compute_iff(expr[1], expr[2])
        elif expr[0] == 'VAR':
            return self.__compute_var(expr[1])
        elif expr[0] == 'TRUE':
            return ('CONSTANT', 1)
        elif expr[0] == 'FALSE':
            return ('CONSTANT', 0)
    
    def compute_ctl(self):
        ctl_parse_tree = parser.parse(self.verify_ctl)
        return self.__compute_expr(ctl_parse_tree)
    

if __name__ == "__main__":
    
    states = ('s0', 's1', 's2', 's3')
    attributes_dict = {'s3': ("q")}
    transitions = (('s0', 's0'), ('s0', 's1'), ('s0', 's2'), ('s1', 's3'), ('s3', 's0'), ('s2', 's1'), ('s2', 's3'))
    verify = ('s0', 'EG ~q->False')
    tsm = TSM(states, attributes_dict, transitions, verify)
    test = tsm.compute_ctl()
    
    print(test)
    
                
        
        
    
    
    
