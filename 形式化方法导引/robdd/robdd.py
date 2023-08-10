class ROBDD:
    def __init__(self):
        self.truth_table = {
            "AND": {"00": 0,"01": 0,"10": 0,"11": 1},
            "OR": {"00": 0,"01": 1,"10": 1,"11": 1},
            "IMPLIES": {"00": 1,"01": 1,"10": 0,"11": 1},
            "IFF": {"00": 1,"01": 0,"10": 0,"11": 1}
        }
        self.all_elements = set()
        self.all_elements.add(('CONSTANT', 1))
        self.all_elements.add(('CONSTANT', 0))
       
    def build(self, parse_tree):
        return self.robdd_build(self.rebuild_parse_tree(parse_tree)) 

    def get_unique_element(self, element):
        if element not in self.all_elements:
            self.all_elements.add(element)
              
        for i in self.all_elements:
            if i == element:
                return i
            
    def rebuild_parse_tree(self, parse_tree):
        if len(parse_tree) == 3:
            parse_tree = (parse_tree[0], self.rebuild_parse_tree(parse_tree[1]), self.rebuild_parse_tree(parse_tree[2]))
        elif len(parse_tree) == 4:
            parse_tree = ('VARIABLE', parse_tree[1], self.get_unique_element(('CONSTANT', 1)), self.get_unique_element(('CONSTANT', 0)))
        return parse_tree
        

    def robdd_build(self, parse_tree):
        if len(parse_tree) == 3:
            if parse_tree[0] in self.truth_table:
                if parse_tree[1][0] == 'CONSTANT' and parse_tree[2][0] == 'CONSTANT':
                    element = ('CONSTANT', self.truth_table[parse_tree[0]][str(parse_tree[1][1])+str(parse_tree[2][1])])
                    return self.get_unique_element(element)
                else:
                    ltree = self.get_unique_element(self.robdd_build(parse_tree[1]))
                    rtree = self.get_unique_element(self.robdd_build(parse_tree[2]))
                    if ltree is rtree:
                        if parse_tree[0] == 'AND' or parse_tree[0] == 'OR':
                            return ltree
                        else:
                            return self.get_unique_element(('CONSTANT', 1))
                    else:    
                        parse_tree = (parse_tree[0], ltree, rtree)
                        if ltree[0] == 'VARIABLE' and rtree[0] == 'VARIABLE':
                            min_var = ltree[1] if ltree[1] < rtree[1] else rtree[1]
                        elif ltree[0] == 'VARIABLE':
                            min_var = ltree[1]
                        else:
                            min_var = rtree[1]
                        if min_var == ltree[1] and min_var == rtree[1]:
                            ltree = self.get_unique_element(self.robdd_build((parse_tree[0], parse_tree[1][2], parse_tree[2][2])))
                            rtree = self.get_unique_element(self.robdd_build((parse_tree[0], parse_tree[1][3], parse_tree[2][3]))) 
                        elif min_var == ltree[1]:
                            ltree = self.get_unique_element(self.robdd_build((parse_tree[0], parse_tree[1][2], parse_tree[2])))
                            rtree = self.get_unique_element(self.robdd_build((parse_tree[0], parse_tree[1][3], parse_tree[2])))
                        else:
                            ltree = self.get_unique_element(self.robdd_build((parse_tree[0], parse_tree[1], parse_tree[2][2])))
                            rtree = self.get_unique_element(self.robdd_build((parse_tree[0], parse_tree[1], parse_tree[2][3])))

                        if ltree is rtree:
                            return ltree
                        else:
                            return self.get_unique_element(('VARIABLE',min_var, ltree, rtree))

        return self.get_unique_element(parse_tree) 


        
        
