#include <vector>
#include <sstream>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <stdio.h>
#include <iostream>
#include <sys/personality.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/user.h>
#include <sys/reg.h>
#include <sys/syscall.h>

using namespace std;

vector<intptr_t>addrs = {0x000055555555516c,0x000055555555516d,0x0000555555555184};
int main(){
  pid_t pid   = fork();
  if(pid ==0){

    personality(ADDR_NO_RANDOMIZE);

  string ex = "l";
  string sh = "./"+ex;
	
	ptrace(PTRACE_TRACEME,0,0,NULL); // set process to be traced 
	execl(sh.c_str(),ex.c_str(),NULL); // run the another process of executable 
  }
  else{
    struct user_regs_struct regs;
    int stat;
    //wait(&stat);
    /*
    ptrace(PTRACE_GETREGS,pid,0,&regs);
    
    printf("\nRIP=0x%llx\n",regs.rip);
    for(int i =0;i<addrs.size();i++){
      cout<<hex<<addrs[i]<<endl;
       auto data = ptrace(PTRACE_PEEKDATA,pid,(void*)addrs[i],0);

      auto comb = (data & ~0xff) | 0xcc;
      ptrace(PTRACE_POKETEXT,pid,addrs[i],comb);
      cout<<data<<":"<<comb<<endl;
      
    }
    
       ptrace(PTRACE_SINGLESTEP,pid,0,0);
    */
    struct user_regs_struct reg;   
    wait(&stat);
    int k=0;
    intptr_t addr= addrs[0];


    auto backed = ptrace(PTRACE_PEEKDATA,pid,addr,nullptr); // get DATA values of this address
    uint64_t shitty = (backed & 0xff);
        uint64_t int3 = 0xcc;
    while(WIFSTOPPED(stat)){
        struct user_regs_struct regs;
        auto data = ptrace(PTRACE_PEEKDATA,pid,addr,nullptr); // get DATA values of this address
        auto datg = ptrace(PTRACE_PEEKDATA,pid,addr-1,nullptr); // get DATA values of this address
        uint64_t datai3 = ((data& ~0xff) | int3); // just change values to add at the end xcc (int 3)
        ptrace(PTRACE_POKEDATA,pid,addr,datai3); // poke data back to memory 
        auto test = ptrace(PTRACE_PEEKDATA,pid,addr,nullptr); // peek data to see if it fits.
        ptrace(PTRACE_GETREGS,pid,0,&regs); // trace all the regs 
        ptrace(PTRACE_SINGLESTEP,pid,0,0);// go instruction-by-instruction
          wait(&stat);

        auto dot = ptrace(PTRACE_PEEKDATA,pid,regs.rip,nullptr); // get DATA values of this address   
         
          if(regs.rip == 0x000055555555518a){
            cout<<"WE ARE DONE HERE";
            exit(1);
          }

        if(regs.rip == addr+1){
          cout<<hex<<regs.rip<<endl;
          auto dat = ptrace(PTRACE_PEEKDATA,pid,regs.rip-1,nullptr);
              
          cout<<dat<<":"<<backed<<endl;
          uint64_t newdat = (dat & ~0xff) | shitty;
          
          ptrace(PTRACE_POKEDATA,pid,regs.rip-1,newdat); // poke data back to memory 
          auto test = ptrace(PTRACE_PEEKDATA,pid,regs.rip-1,nullptr); // peek data to see if it fits.
          cout<<test<<endl;
          reg.rip = addrs[2];
          ptrace(PTRACE_SETREGS,pid,0,&reg);
          
          ptrace(PTRACE_SINGLESTEP,pid,0,0);
          wait(&stat);
          
        }
 
	  }



 }
 

}
