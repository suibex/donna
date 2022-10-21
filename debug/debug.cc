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

using namespace std;

class WeakAssDebugger{
	public:
	bool debug;
	pid_t address;
	string filename;
	vector<unsigned int>addresses;
	WeakAssDebugger(vector<unsigned int> p ,string fn,bool debug_option){
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
		cout<<"cmon nigger."<<endl;
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
	Banker *bank = new Banker(bf,true);
	vector<pair<unsigned int ,unsigned int>> instr =   bank->banker_load_file(filename);
//	WeakAssDebugger *deb =new WeakAssDebugger(data,"./test_exec",true);
}

