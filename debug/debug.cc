#include <stdio.h>
#include <string.h>
#include <sys/ptrace.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>
#include <iostream>
#include <fstream>

using namespace std;

class WeakAssDebugger{
	public:
	bool debug;
	pid_t address;
	WeakAssDebugger(string filename,bool debug_option){
		debug=debug_option;
		pid_t pid = fork();
		address=pid;	
		if(pid ==0 ){
			printf("*** donna started process with pid:%d \n",getpid());
			ptrace(PTRACE_TRACEME,0,0,NULL);
			execl(filename.c_str(),filename.c_str(),NULL);
			exit(1);
		}
	}

};


//testing purpouses
int main(){
		
	WeakAssDebugger *deb =new WeakAssDebugger("./test_exec",true);
	

}
