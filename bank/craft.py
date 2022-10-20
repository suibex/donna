import os,io,sys

dn =lambda x, n: format(x, 'b').zfill(n)

#test =0x5280028d
#test = 0x8d028052
test = 0x5280028d
n = test
num = dn(n,32)
print(num)


mov = "x10x00101xxiiiiiiiiiiiiiiiiddddd"

#print("x00x 0010 1xxi iiii iiii iiii iiid dddd  -  movn Rd HALF")
#opcodes=[int(num[:4],2),int(num[4:8],2)]
#val = num[10:27]
value=""
reg= ""
for i in range(len(mov)):
  if(mov[i] == 'i'):
    value+=num[i]
  if(mov[i] == 'd'):
    reg+=num[i]

reg = int(reg,2)
value = int(value,2)
print(reg,value)

