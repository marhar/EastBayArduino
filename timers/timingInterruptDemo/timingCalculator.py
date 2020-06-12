#!/Users/marhar/anaconda2/bin/python

#clock=8000000
clock=16000000

prescalers=[1,8,64,256,1024]

ocra_hi = 256

# // ocraval = clock/(hz*scaler) -1
# // hz = clock/(scaler*ocra) -1

for prescaler in prescalers:
    for ocra in range(1,ocra_hi):
        hz = clock/(prescaler*ocra)-1
        print hz,prescaler,ocra
