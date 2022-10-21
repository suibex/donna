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
#include <sys/uio.h>
#define ARM 0x00
#define X86 0x01
#define __u64 uint64_t
#define NT_PRSTATUS 1
using namespace std;
//copied from linux/ptrace.h <-- for some reason , macOS devs haven't finished this struct.
//apparently they do not like debugging. only closed source code and zero knowledge of the user. why cook?
//macos is actually a really fun os to use , just closed source thing kinda f's it up.
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
	void save_register_data(struct user_pt_regiz a,unsigned int b ){
		string file = "../data/reginfo/"+filename+to_string(b)+".reginfo";
		string data;
		for(int i =0;i<32;i++){
			data+="w"+to_string(i)+":\t"+to_string(a.regs[i])+"\n";
		}
		data+="pc:\t"+to_string(a.pc)+"\n";
		data+="sp:\t"+to_string(a.sp)+"\n";
		data+="pstate:\t"+to_string(a.pstate);
		ofstream str(file);
		str<<data;
		str.close();
		if(debug){
			printf("*** register data saved. addr:%lx\n",b);
		}
	}
	void cycle_through_instructions(pid_t addr){
		uint64_t brk=0;
		if(arch == ARM){
		 brk = 0xd4200020;//if running on ARM, gotta change this if arch is not arm.
		}	
		
		int stat;
		int bab =0;
	       	vector<unsigned int> old_data;	
		wait(&stat);
		while(WIFSTOPPED(stat)){
			if(bab==addresses.size()){
				break;
			}
			unsigned int old=0;
			struct user_pt_regiz regs;
			struct iovec io;
			io.iov_base = &regs;
			io.iov_len = sizeof(regs);
			/*//idk why this doesnt work but ok.
			auto data = ptrace(PTRACE_PEEKDATA,addr,addresses[bab],nullptr);
			old = data;
			if(arch == ARM){
				data = brk;
			}
			ptrace(PTRACE_POKEDATA,addr,addresses[bab],data);
			*/
			//data = ptrace(PTRACE_PEEKDATA,addr,addresses[bab],nullptr);
			//printf("\nDATA:%x",data);
			if(ptrace(PTRACE_GETREGSET,addr,(void*)NT_PRSTATUS,(void*)&io)==-1){
				printf("\nbad request.");
				exit(1);
			}
			save_register_data(regs,bab);
			ptrace(PTRACE_PEEKDATA,addr,addresses[bab],old);
			ptrace(PTRACE_SINGLESTEP,addr,0,0);
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
			execl(filename.c_str(),("./"+filename).c_str(),NULL);
	//		exit(1);
		}
		else{
			cycle_through_instructions(pid);
			printf("*** donna's register catching done.\n");
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

	if(bank->debug){
		printf("*** saving decompiled data to 'data' folder.\n");
	}

	ofstream str("../data/decompiled"+filename+".dn");
	str<<decompiled;
	str.close();
	WeakAssDebugger *deb =new WeakAssDebugger(address_savings,filename,false,ARM);
	deb->start_debug();
	//ok at this point, we god decompiled binary, we got register data. lets see where these instructions are located in the actual file. this will be done in python
	//c++ stuff done for now .great , atleast for basic arm binaries ( current goal )
}
int main(int argc,char *argv[]){
	//printf("*** donna v4.0 , completely rewritten. works ok with small binaries. do not try anything fancy here.");
	if(argc< 2){
		printf("*** donna debugger v4.0\n\tprovide filename of executable\n\tprovide bank file for architecture (example. arm64.bank)\n");
		exit(1);
	}
	string filename  =argv[1];
	string arch = argv[2];
	donna_main(filename,arch);
}

