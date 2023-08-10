from parse import parser
from robdd import ROBDD

# # 解析表达式

# expr = "p & p"

# expr = "a & b | c"

# expr = "a & b | ~b & c -> a <-> c"

expr = "(p -> r) & (q <-> (r | p))"

# expr = "(p1 -> p3) & (p2 <-> (p3 | p1))"

# expr = "~p0 & ~p1 & ~p2 & ~p3 | p0 & p1 & ~p2 & ~p3 | p0 & ~p1 & ~p2 & p3 | p0 & ~p1 & p2 & p3 | ~p0 & p1 & p2 & p3 | ~p0 & ~p1 & ~p2 & p3 | ~p0 & ~p1 & p2 & ~p3"

parse_tree = parser.parse(expr)

robdd_tree = ROBDD().build(parse_tree)

print(robdd_tree)
