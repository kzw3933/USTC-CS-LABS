import sys
sys.path.append("/home/jack/Desktop/class-labs/fm/z3str/")

from zvar import Var
from zstr import Str
from constraint import Equation
from driver import Driver

str1 = Str() 
v1 = Var("v1")

str1.append(v1)
str1.append(v1)
str1.append(v1)

str2 = Str()
v2 = Var("v2")
str2.append(v2)


eqc1 = Equation(str1, str2)

driver = Driver(10)
driver.append_constraints([eqc1])
driver.solve()