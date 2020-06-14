#!/Users/marhar/anaconda2/bin/python

clock=16000000
prescalers=[1,8,64,256,1024]
ocra_hi = 256

# ocra = clock/(hz*scaler) -1
# hz = clock/(scaler*ocra) -1

import collections

Solution = collections.namedtuple("Solution","hz prescaler ocra")

def all():
  a=[]
  for prescaler in prescalers:
    for ocra in range(1,ocra_hi):
      hz = clock/(prescaler*ocra)-1
      a.append(Solution(hz,prescaler,ocra))
  return sorted(a)


def guess(target):
  a=[]
  lo = target - target * .1
  hi = target + target * .1
  for i in all():
    if i[0] > lo and i[0] < hi:
      a.append(i)
  return a



#all()
for i in guess(1000): print i
