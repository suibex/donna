import os,io,sys


dn =lambda x, n: format(x, 'b').zfill(n)

def log(test):
    n = test
    num = dn(n,32)
    for i in range(32):
        if( i%4==0):
            print(" ",end="") 
        print(num[i],end="")
    print("")


#test =0x5280028d
#test = 0x8d028052
log(0xaa1e03e1)
log(0xeb01005f)
log(0xeb1403df)
