#include <stdio.h>
#include <string.h>
#include <sys/ptrace.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/personality.h>
#include <vector>
#include <iostream>
#include <fstream>
#include "load_bank.h"
#include <sys/wait.h>
#include <stdint.h>
#include <sstream>
#define ARM 0x00
#define X86 0x01
#define __u64 uint64_t
using namespace std;



struct user_pt_regiz{
	
__u64 	regs [31];
__u64 	sp;
 
__u64 	pc;
 
__u64 	pstate;

};

class WeakAssDebugger{
	public:
	bool debug;
	pid_t address;
	string filename;
	int arch;
	vector<unsigned int>addresses;

	WeakAssDebugger(vector<unsigned int> p ,string fn,bool debug_option,int a){
		arch =a; 
		debug=debug_option;
		filename = fn;
		for(int i =0;i<p.size();i++){
			addresses.push_back(p[i]);
		}
		if(debug){
			printf("*** donna loaded decompiled data.\n");
		}
	}

	void cycle_through_instructions(pid_t addr){
	//	cout<<"cmon nigger."<<endl;
//		int stat;
//		i
//
		uint64_t brk=0;
		if(arch == ARM){
		 brk = 0xd4200020;//if running on ARM, gotta change this if arch is not arm.
		}	
		
		int stat;
		wait(&stat);
		//wait(&stat);
		int bab =0;
	       vector<unsigned int> old_data;	
		while(WIFSTOPPED(stat)){
			if(bab==addresses.size()){
				break;
			}
		
			struct user_pt_regiz regs;
			struct iovec io;
			io.iov_base = &regs;
			io.iov_len = sizeof(regs);
			printf("\naddress:%x",addresses[bab]);
			auto data = ptrace(PTRACE_PEEKDATA,addr,addresses[bab],nullptr);
		
			if(arch == ARM){
				data = brk;
			}
			ptrace(PTRACE_POKEDATA,addr,addresses[bab],data);
			data = ptrace(PTRACE_PEEKDATA,addr,addresses[bab],nullptr);
			//printf("\nDATA:%x",data);
			unsigned char*bdb=(unsigned char*)malloc(20+sizeof(user_pt_regiz));
			memset(bdb,0,20+sizeof(user_pt_regiz));
			ptrace(PTRACE_GETREGSET,addr,0,bdb);	
			ptrace(PTRACE_SINGLESTEP,addr,0,0);
				
			for(int i =0;i<sizeof(user_pt_regiz)+20;i++){
				if(i%10==0){
					printf("\n");

				}
				if(bdb[i] == 0x14){
					printf("\nwhat?");
				}
				printf(" %x ",bdb[i]);

			}
			free(bdb);

		//	exit(1);
		//
			//
		//cout<<regs.pc<<endl;
			wait(&stat);
			bab++;
		}
	}
	void start_debug(){
		pid_t pid = fork();
		address=pid;	
		if(pid ==0 ){
			personality(ADDR_NO_RANDOMIZE);
			printf("*** donna started process with pid:%d \n",getpid());
			ptrace(PTRACE_TRACEME,0,0,NULL);
			execl(filename.c_str(),filename.c_str(),NULL);
	//		exit(1);
		}
		else{
			cycle_through_instructions(pid);
			
		}
		
	}
	
	
};


//testing purpouses
void donna_main(string filename,string bf){
	//cout<<bf<<endl;
	Banker *bank = new Banker(bf,false);
	vector<pair<unsigned int ,unsigned int>> instr =   bank->banker_load_file(filename);
	string decompiled;
	stringstream ss;
	//decompiling actual file and saving the addresses for later use (debugging)
	vector<unsigned int> address_savings;
	for(int i =0 ;i<instr.size();i++){
		unsigned int addr = instr[i].first;
		unsigned int hex = instr[i].second;
		string res= bank->opcode_decode(hex);
		ss.clear();
		string address;
		char a[64];
		sprintf(a,"%x",addr);
		address=a;
		address ="0x"+address+":\t"+res;
		decompiled+=address+"\n";
		address_savings.push_back(addr);
	}			
	cout<<"***********************************************"<<endl;
	cout<<decompiled<<endl;
	if(bank->debug){
		printf("*** saving decompiled data to 'data' folder.\n");
	}

	ofstream str("../data/decompiled"+filename+".dn");
	str<<decompiled;
	str.close();
	WeakAssDebugger *deb =new WeakAssDebugger(address_savings,filename,true,ARM);
	deb->start_debug();
	
}
int main(int argc,char *argv[]){
	printf("*** donna v4.0 , completely rewritten. works ok with small binaries. do not try anything fancy here.");
	string filename  =argv[1];
	string arch = argv[2];
	donna_main(filename,arch);
}

