import sys
sys.path.append("/home/jack/Desktop/class-labs/fm/z3str/")

from zvar import Var
from zstr import Str
from constraint import LenCons, Equation
from driver import Driver

str1 = Str() 
v1 = Var("v1")
str1.append("a")
str1.append(v1)
str1.append("b")

str2 = Str()
v2 = Var("v2")
str2.append(v2)
str2.append("b")

str3 = Str()
str3.append(v2)
str3.append(Var("v3"))

str4 = Str()
str4.append(Var("v4"))


lenc_v1 = LenCons([v1.get_len_var()], "==", [5])
lenc_v2 = LenCons([v2.get_len_var()], "==", [6])

eqc1 = Equation(str1, str2)
eqc2 = Equation(str3, str4)

driver = Driver(10)
driver.append_constraints([lenc_v1, lenc_v2, eqc1, eqc2])
driver.solve()