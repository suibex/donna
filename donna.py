"""
*******************************************************************************************************************************************

		donna disassembler v1.1
			-web server connector

		Last update commited at: Mar 21 22.

		Created by nitrodegen.
		In labs of cyfly Computer Corporation.

	WARNING: ANY USE OF THIS CODE FOR PATENT OR PROFITABLE USE OR REUPLOAD OF SOURCE IS STRONGLY RESTIRCED, AND WILL BE HELD UNDER LAW!
	NOTE: ONLY USE THIS CODE TO CHANGE AND UPDATE THE BRANCH FOR THE MAIN REPO, NEVER FOR YOURSELF!

******************************************************************************************************************************************
"""

import os,io,sys
from multiprocessing import Process
from time import sleep

def remove():
  
    os.chdir("./data")
    dat = os.listdir()
    for i in dat:
        os.remove(i)


def web():
    os.chdir("./host")
    os.system(f"python3 server.py {dir}")

    print("Server closed.")
    remove()

if(len(sys.argv)<2):
    print("********** donna disassembler webserver **********\n\ti'm not into you, i'm donna :)\nHELP:\n\tThis is what i call a fancier way to use this tool.\n\tRun it like this: sudo python3 donna.py [filename]\n\tSoon after that you will be dropped into 'donna shell' and you can set stuff from there.")
    sys.exit(1)

t =Process(target = web)
t.daemon = True

def cmdLine():
    global t
    print("********** donna disassembler shell ************\n\tTHIS SHELL IS USED TO SET BREAKPOINT EASIER WHILE RUNNING ON THE WEB.\n\tSet the breakpoint with :breakpoint,br,b and provide the address.") 
    while True:
        msg = input("<donna>")
        if("list" in msg):
            d = os.listdir("data")
            for i in range(len(d)):
                if(".reg" in d[i]):
                    print(d[i])
        if("breakpoints" in msg or "b" in msg or "break" in msg):
            add = msg.split("*")
            d = os.listdir("data")
            if(add[1] in d):
                f = open("./data/"+add[1],"r")
                cont = f.read()
                print(cont)
        if("clear" ==msg):
            
            for i in range(25):
                print("\n                            \n")
            print("********** donna disassembler shell ************\n\tTHIS SHELL IS USED TO SET BREAKPOINT EASIER WHILE RUNNING ON THE WEB.\n\tSet the breakpoint with :breakpoint,br,b and provide the address.") 
        if(msg=="exit"):
            remove()
            quit()
        if(msg=="remove"):
            remove()
            print("Removed all data from previous dumps.")
            
            


dir = sys.argv[1]
print(os.getcwd())
os.system(f"./donna -d {dir}")
sleep(0.5)
print("Disassembled.")


t.start()
sleep(0.5)

cmdLine()
   


  








