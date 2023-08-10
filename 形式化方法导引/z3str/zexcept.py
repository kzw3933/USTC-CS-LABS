
class EquivalentConflict(Exception):
    def __init__(self, message="等价类存在冲突"):
        super().__init__(message)
        self.message = message
        

class SystemOfEquationsConflict(Exception):
    def __init__(self, message="方程组存在冲突,无法求出满足长度约束的解"):
        super().__init__(*message)
        self.message = message

class Divisible(Exception):
    def __init__(self, message="无法除整"):
        super().__init__(*message)
        self.message = message
        
class OverBound(Exception):
    def __init__(self, message="字符串变量的长度达到边界"):
        super().__init__(*message)
        self.message = message