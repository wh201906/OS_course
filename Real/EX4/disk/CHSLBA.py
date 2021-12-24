from z3 import *

HPC = Int('HPC')
SPT = Int('SPT')
s = Solver()

C = 7
H = 254
S = 63
LBA = 128519
s.add((C * HPC + H) * SPT + S - 1 == LBA)

C = 0
H = 1
S = 1
LBA = 63
s.add((C * HPC + H) * SPT + S - 1 == LBA)

result = str(s.check())
print(result)
if(result == 'sat'):
    print(s.model())