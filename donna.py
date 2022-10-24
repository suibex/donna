"""
      *part of nitrodegen's god tool for debugging aarch64 based systems.*
        donna backend and frontend wrapper
            this module is used to actually run the app and everything else
        
        (C) Gavrilo Palalic 2022
"""
import os,io,sys

if(len(sys.argv) < 2):
    print("***** donna v4.0 *****\n\tARM ELF debugger and analyzer. (maybe expanded in future)")
    print("how to use:\nprovide filename of the file you want to analyze.\nexample: python3 donna.py <filename> ")
    exit(1)

def file_markings(filename,dec):
    BASE_ADDR = 0x400000 #TODO:actually find the address again, don't just try to predict.
    #but im lazy so whatever.
    d = open(filename,"rb")
    
    d = d.read()
    addresses=[]
    dec = dec.split("\n")
    for x in dec:
        if(len(x) > 1):
            x=x.split(":")[0].replace("0x","")
            x = int(x,base=16)
            addresses.append(x-BASE_ADDR)
    #exit(1)
    analyzed=""
    result=""
    
    for byte in d:
        if(byte <0x22 or byte>0xd088):
            byte="."
        else:
            byte=chr(byte)
        analyzed+=byte
   
   
    for i in range(len(analyzed)):
        bb = analyzed[i]
        for j in range(len(addresses)):
            if(i == addresses[j]):                    
                bb=""
                
                for z in range(4):

                    bb+=analyzed[addresses[j]+z]
                bb="<mark class='red'>"+bb+"</mark>" 
        result+=(bb)
    return result

def read_config_data(filename):
    analysis=None
    reginfo=[]
    data = os.listdir("./data")
    print("*** reading analysis...")
    for file in data: 
        #print(file)
        if(file != "reginfo"):
            if(".dn" in file):
                fname = file.split("decompiled")[1].replace(".dn","")
                if(filename == fname):
                    d = open("./data/"+file)
                    d =d.read()
                    analysis = d

        else:
            d = os.listdir("./data/reginfo")
            for x in d:
                fdd = x.split(".")
                fname=fdd[0]
                if(filename in fname):
                    z = open("./data/reginfo/"+x)
                    z =z.read()                    
                    part = fname.split(filename)[1]
                    part=int(part)
                   # print(part)
                    reginfo.append((z,part))

    for i in range(len(reginfo)):
        for j in range(len(reginfo)):
            if(reginfo[i][1] < reginfo[j][1]):
                t = reginfo[i]
                reginfo[i] = reginfo[j]
                reginfo[j] =t
    reg=[]
    for i in range(len(reginfo)):
        reg.append(reginfo[i][0])
    
    reginfo= reg
    
    return [analysis,reginfo]
os.system("cd tooling && ./cleanup.sh && cd ../")
sys.path.insert(1, './web')
import dweb
filename = sys.argv[1]
data=read_config_data(filename)
markings = file_markings(filename,data[0])
os.system("./backend "+filename +" ./banks/arm64.bank")
data= (filename,data[0],data[1],markings)
dweb.data=data
wb = dweb.DonnaWebServer()
wb.run_server()

