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


vector<intptr_t>addrs = {0x000055555555516c,0x000055555555516d,0x0000555555555170,0x0000555555555177,0x000055555555517a,0x000055555555517f,0x0000555555555184};
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
      vector<long long unsigned int >backedbytes; 
    struct user_regs_struct reg;   
  
    bool hop=false;
    int k=0;
    
    uint64_t int3 = 0xcc;
    wait(&stat);

    while(WIFSTOPPED(stat)){
        struct user_regs_struct regs;
        intptr_t addr  = addrs[k];
        if(hop == false){

          for(int i =0;i<addrs.size();i++){
                //cout<<hex<<addrs[i]<<endl;
                auto data = ptrace(PTRACE_PEEKDATA,pid,(void*)addrs[i],0);
                if(hop ==false){
                    backedbytes.push_back(data);
                    cout<<i<<" | DAT " <<hex<<data<<endl;
                 }
               
                if(i+1==addrs.size() && hop == false){     
                  hop=true;
                 }
                auto comb = (data & ~0xff) | 0xcc;
                ptrace(PTRACE_POKEDATA,pid,addrs[i],comb);
                
               // cout<<data<<":"<<comb<<endl;
                
              }
        }  
        
        




        ptrace(PTRACE_GETREGS,pid,0,&regs); // trace all the regs 
       
        ptrace(PTRACE_SINGLESTEP,pid,0,0);// go instruction-by-instruction
        wait(&stat);
        auto dot = ptrace(PTRACE_PEEKDATA,pid,regs.rip,nullptr); // get DATA values of this address   
        //cout<<hex<<addrs[k]<<":"<<hex<<regs.rip<<":"<<stat<<endl;
         
        if(k>1){
          cout<<hex<<regs.rip<<endl;
         }
        if(regs.rip == addrs[addrs.size()-1]){
          cout<<"Program executed successfully."<<endl;
          exit(1);
         }
        if(regs.rip == addrs[k] && regs.rip != addrs[addrs.size()-1]){
           
            cout<<"=======+"<<regs.rip<<"+======"<<endl;
            /*  
             *  HOW TO GO ABOUT THIS PROCESS;
             *  REMOVE 0xcc FROM CODE 
             *  SET EIP TO BE BEFORE:
             *
             */
            auto data =ptrace(PTRACE_PEEKDATA,pid,regs.rip,0);
           
            ptrace(PTRACE_POKEDATA,pid,regs.rip,backedbytes[k]);
            auto mess =ptrace(PTRACE_PEEKDATA,pid,regs.rip,0);
            cout<<hex<<mess<<endl;
            if(addrs[k-1] > 100){
                reg.rip = addrs[k-1];
             }
            else{
                reg.rip = regs.rip-4;
            }
            cout<<hex<<reg.rip<<endl;
            ptrace(PTRACE_SETREGS,pid,0,&reg);
            k++;
            ptrace(PTRACE_SINGLESTEP,pid,0,0);
          //  wait(&stat);

         

            //end
           

         }
        if(k == addrs.size()){
          k=0;
         }
        
 
	  }
// CURRENT STATUS :
//  I GET FUCKING SIGSEGV 
//  (SEGFAULT)
//


 }
 

}
