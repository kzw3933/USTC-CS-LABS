import sys
sys.path.append("/home/jack/Desktop/class-labs/fm/z3-str*/")

from zvar import Var
from zstr import Str
from constraint import Equation
from driver import Driver

str1 = Str()
v3 = Var("v3")
str1.append(v3)
str1.append("ab")
v2 = Var("v2")
str1.append(v2)
str1.append("cd")


str2 = Str()
str2.append("caab")
v1 = Var("v1")
str2.append(v1)

eqc = Equation(str1,str2)
driver = Driver(2)
driver.append_constraints([eqc])
driver.solve()