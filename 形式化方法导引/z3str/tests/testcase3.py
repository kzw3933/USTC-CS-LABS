import sys
sys.path.append("/home/jack/Desktop/class-labs/fm/z3-str*/")

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


lencX = LenCons([v1.get_len_var()], "==", [6])
lencY = LenCons([v2.get_len_var()], "==", [6])

eqc1 = Equation(str1, str2)

driver = Driver(5)
driver.append_constraints([lencX, lencY, eqc1])
driver.solve()