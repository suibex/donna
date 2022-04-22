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
  
    os.chdir("/var/tmp/donna")
    dat = os.listdir()
    for i in dat:
        os.remove(i)


def web(fi):
   
    os.chdir("./host")
    
    os.system(f"python3 server.py {fi}")
    print("Server closed.")
    remove()

if(len(sys.argv)<2):
    print("********** donna disassembler webserver **********\n\ti'm not into you, i'm donna :)\nHELP:\n\tThis is what i call a fancier way to use this tool.\n\tRun it like this: sudo python3 donna.py [filename]\n\tSoon after that you will be dropped into 'donna shell' and you can set stuff from there.")
    sys.exit(1)

t =Process(target = web,args=(sys.argv[1],))
t.daemon = True
def cmdLine():
    global t
    print("********** donna disassembler shell ************\n\tTHIS SHELL IS USED TO SET BREAKPOINT EASIER WHILE RUNNING ON THE WEB.\n\tSet the breakpoint with :breakpoint,br,b and provide the address.") 
    while True:
        msg = input("<donna>")
        if("list" in msg):
            d = os.listdir("/usr/tmp/donna")
            for i in range(len(d)):
                if(".reg" in d[i]):
                    print(d[i])
        if("breakpoints" in msg or "b" in msg or "break" in msg):
            add = msg.split("*")
            d = os.listdir("/usr/tmp/donna/")
            if(add[1] in d):
                f = open("/usr/tmp/donna/"+add[1],"r")
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
            
            

if(os.geteuid() !=0):
    print("donna has to be run as root.\nre-run donna with sudo!")
    sys.exit(1)

else:
    if(len(sys.argv)>=2):
        os.system("mkdir /var/tmp/donna")
        dirz = sys.argv[1]
        print(os.getcwd())
        os.system("echo 0 > /proc/sys/kernel/randomize_va_space")

        os.system(f"./donna-debug -d {dirz}")
        sleep(0.5)
        os.system("echo 1 > /proc/sys/kernel/randomize_va_space")
        print("Disassembled.")
       
        t.start()
        sleep(0.5)

        cmdLine()
    else:
        os.system("./donna-debug")

           


  








