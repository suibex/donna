"""
*******************************************************************************************************************************************

		donna disassembler v1.1
			-web server connector

		Last update commited at: Mar 17 22.

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
        if("breakpoint" in msg or "b" in msg or "break" in msg):
            add = msg.split("*")
            os.system(f"./donna -b {sys.argv[2]} {add[1]}")
            print("Breakpoint saved.")
        if("clear" ==msg):
            
            for i in range(25):
                print("\n                            \n")
            print("********** donna disassembler shell ************\n\tTHIS SHELL IS USED TO SET BREAKPOINT EASIER WHILE RUNNING ON THE WEB.\n\tSet the breakpoint with :breakpoint,br,b and provide the address.") 
        if(msg=="exit"):
            quit()
        if(msg=="remove"):
            remove()
            print("Removed all data from previous dumps.")
            
            
if(sys.argv[1] == "-d"):

    dir = sys.argv[2]
    print(os.getcwd())
    os.system(f"./donna -d {dir}")
    sleep(0.5)
    print("Disassembled.")
    f = open("./data/reg.registers","wb")
    f.write(b"breakpoints appear here")
    f.close()
    
    t.start()
    sleep(0.5)
    
    cmdLine()
   


  








