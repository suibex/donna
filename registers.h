
/*
*******************************************************************************************************************************************

		donna disassembler v1.1
			-register dump module

		Last update commited at: Mar 17 22.

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
using namespace std;
void breakDump(pid_t pid,intptr_t addr){
	long long unsigned int rax,rip,rcx,rbx,rdx,rsi,rdi,rbp,rsp,orax;
	int stat;
	// make struct of regs 

	uint64_t int3 = 0xcc;

	wait(&stat); // wait to see what will stack do with stat 
	while(WIFSTOPPED(stat)){
		struct user_regs_struct regs;
		auto data = ptrace(PTRACE_PEEKDATA,pid,addr,nullptr); // get DATA values of this address

		uint64_t datai3 = ((data& ~0xff) | int3);
		ptrace(PTRACE_POKEDATA,pid,addr,datai3);
		auto test = ptrace(PTRACE_PEEKDATA,pid,addr,nullptr);
		if(regs.rip == addr){
			cout<<"Breakpoint at "<<"0x"<<hex<<regs.rip<<endl;

			
		}
		
		ptrace(PTRACE_GETREGS,pid,0,&regs); // trace all the regs 
		ptrace(PTRACE_SINGLESTEP,pid,0,0);// go instruction-by-instruction
		

		rax=regs.rax;
		rip=regs.rip;
		rcx=regs.rcx;
		rbx=regs.rbx;
		rdx=regs.rdx;
		rsi=regs.rsi;
		rdi=regs.rdi;
		rbp=regs.rbp;
		rsp=regs.rsp;
		orax=regs.orig_rax;
		wait(&stat);
		if(regs.rip == addr+1){
			break;
		}
	}
	//stack pointer does change with execution , cause memory moves arround :) 
	if(rax ==231){
		cout<<"\nCould not breakpoint at given address."<<endl;
		exit(1);
	}
	printf("\nrax\t0x%llx\t%lld\nrbx\t0x%llx\t%lld\nrcx\t0x%llx\t%lld\nrdx\t0x%llx\t%lld\nrsi\t0x%llx\t%lld\nrdi\t0x%llx\t%lld\nrbp\t0x%llx\t%lld\nrip\t0x%llx\t%lld\nrsp\t0x%llx\t%lld\n",rax,rax,rbx,rbx,rcx,rcx,rdx,rdx,rsi,rsi,rdi,rdi,rbp,rbp,rip,rip,rsp,rsp);
	char b[99999];
	stringstream ss;
	ss<<"0x"<<hex<<addr;
	string atloc;
	ss>>atloc;
	ss.clear();
	

	sprintf(b,"Breakpoint at %s\nrax\t0x%llx\nrbx\t0x%llx\nrcx\t0x%llx\nrdx\t0x%llx\nrsi\t0x%llx\nrdi\t0x%llx\nrbp\t0x%llx\nrip\t0x%llx\nrsp\t0x%llx\n",atloc.c_str(),rax,rbx,rcx,rdx,rsi,rdi,rbp,rip,rsp);
	string dumper = b;
	

	ofstream file("./data/reg.registers");
	if(file.is_open()){
		file<<dumper;

	}
	else{
		cout<<"Could not open stream."<<endl;
		exit(1);
	}
	file.close();

}

void RunExec(string ex){
	string sh = "./"+ex;
	
	ptrace(PTRACE_TRACEME,0,0,NULL); // set process to be traced 
	execl(sh.c_str(),ex.c_str(),NULL); // run the another process of executable 
}


void checkstuff(intptr_t addr,string exec){

	
	
	auto pid = fork();
	if(pid ==0){
		personality(ADDR_NO_RANDOMIZE);
		RunExec(exec); // first start the process on another pid and start tracing 
	}
	else{
		if(addr==0){
			
			cout<<"Provide the address."<<endl;
			exit(1);
			
		}
		else{
			breakDump(pid,addr);//run the debugger itself
			
		}
	}
	
}