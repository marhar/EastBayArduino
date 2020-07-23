import resistors
# Rt = 1/(1/r1+1/r2...)


values = [a[0] for a in resistors.available]

results=[]
for r1 in values:
    for r2 in values:
      for r3 in values:
        results.append((int(1.0/(1.0/r1)), r1))
        results.append((int(1.0/(1.0/r1+1.0/r2)), r1, r2))
        results.append((int(1.0/(1.0/r1+1.0/r2+1.0/r3)), r1, r2, r3))

import pprint
# should be 1.5k , is 10k
print len(results)
print len(set(results))
pprint.pprint(sorted(set(results)))
