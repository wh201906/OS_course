from z3 import *

n = BitVec('n', 8)
s = Solver()

s.add(n & (n - 1) == 0)

while s.check() == sat:
    val = s.model()[n]
    print(val)
    s.add(n != val)

# {1, 2, 4, 8, 16, 32, 64, 128}
