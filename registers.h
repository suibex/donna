
/*
*******************************************************************************************************************************************

		donna disassembler v2.0
		-register dump module

		Last update commited at: April 22 22.
		Created by nitrodegen.
		In labs of cyfly Computer Corporation.

	WARNING: ANY USE OF THIS CODE FOR PATENT OR PROFITABLE USE OR REUPLOAD OF SOURCE IS STRONGLY RESTIRCED, AND WILL BE HELD UNDER LAW!
	NOTE: ONLY USE THIS CODE TO CHANGE AND UPDATE THE BRANCH FOR THE MAIN REPO, NEVER FOR YOURSELF!

******************************************************************************************************************************************
*/
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
#define ll long long unsigned int

using namespace std;

void SaveFile(ll rip , ll rax, ll rcx, ll rbp, ll rsp, ll rbx, ll rdx ,ll rsi, ll rdi){
  char b[9999];
  stringstream ss;
	ss<<"0x"<<hex<<rip;
	string atloc;
	ss>>atloc;
	ss.clear();
	sprintf(b,"Breakpoint at %s\nrax\t0x%llx\nrbx\t0x%llx\nrcx\t0x%llx\nrdx\t0x%llx\nrsi\t0x%llx\nrdi\t0x%llx\nrbp\t0x%llx\nrip\t0x%llx\nrsp\t0x%llx\n",atloc.c_str(),rax,rbx,rcx,rdx,rsi,rdi,rbp,rip,rsp);
	string dumper = b;
	ss.clear();
	ss<<rip;
	string ad;
	ss>>ad;
	ss.clear();

	ofstream file("/var/tmp/donna/"+ad+".reg");
	if(file.is_open()){
		file<<dumper;
	}
	file.close();
  cout<<"saved"<<endl;
  
 }
void breakDump(pid_t pid,vector<intptr_t> addrs){  
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

              }
        }
        ptrace(PTRACE_GETREGS,pid,0,&regs); // trace all the regs
        if(stat == 0xb7f){
           
          cout<<"["<<pid<<"] SIGSEGV (RIP = "<<regs.rip<<")"<<endl; 
            if(regs.rip == 0x7ffff7f0ec84){
                cout<<"printf crashed god damn it."<<endl;
             }
             exit(1);   
        
        }
        ptrace(PTRACE_SINGLESTEP,pid,0,0);// go instruction-by-instruction
 
        auto dot = ptrace(PTRACE_PEEKDATA,pid,regs.rip,nullptr); // get DATA values of this address

        if(regs.rip == addrs[addrs.size()-1]){
            cout<<"Program executed successfully."<<endl;
            break;
         }
        
        if(regs.rip == addrs[k] && regs.rip != addrs[addrs.size()-1]){
            
            auto data =ptrace(PTRACE_PEEKDATA,pid,regs.rip,0);
            SaveFile(regs.rip,regs.rax,regs.rcx,regs.rbp,regs.rsp,regs.rbx,regs.rdx,regs.rsi,regs.rdi);
            ptrace(PTRACE_POKEDATA,pid,regs.rip,backedbytes[k]);
            auto mess =ptrace(PTRACE_PEEKDATA,pid,regs.rip,0);
           if(addrs[k-1] > 100){
                reg.rip = addrs[k-1];
             }
            else{
              reg.rip = regs.rip-4;
            }
            ptrace(PTRACE_SETREGS,pid,0,&reg);
            k++;
            ptrace(PTRACE_SINGLESTEP,pid,0,0);
            wait(&stat);
            //end
            continue;

         }
         wait(&stat);
         
        
        if(k == addrs.size()){
          k=0;
         }
  }
   
}


void RunExec(string ex){
	string sh = "./"+ex;
	
	ptrace(PTRACE_TRACEME,0,0,NULL); // set process to be traced 
	execl(sh.c_str(),ex.c_str(),NULL); // run the another process of executable 
}


void registerdump(vector<intptr_t> addrs,string exec){
  personality(ADDR_NO_RANDOMIZE);
	
  auto pid = fork();
	if(pid ==0){
		personality(ADDR_NO_RANDOMIZE);
		RunExec(exec); // first start the process on another pid and start tracing 
	}
	else{
      breakDump(pid,addrs);	
  }
}
