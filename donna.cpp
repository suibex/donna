/*
*******************************************************************************************************************************************

		donna disassembler v3.0
			-main module

		Last update commited at: June 25 22
		Created by nitrodegen.
		In labs of cyfly Computer Corporation.

	WARNING: ANY USE OF THIS CODE FOR PATENT OR PROFITABLE USE OR REUPLOAD OF SOURCE IS STRONGLY RESTIRCED, AND WILL BE HELD UNDER LAW!
	NOTE: ONLY USE THIS CODE TO CHANGE AND UPDATE THE BRANCH FOR THE MAIN REPO, NEVER FOR YOURSELF!

******************************************************************************************************************************************
*/
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <vector>
#include <unistd.h>
#include <sstream>
#include <sys/mman.h>
#include <random>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>
#include <unistd.h>
#include <elf.h>
#include "dump.h"
#include "registers.h"
using namespace std;
int START=0;
int END=0;
bool aslr=false;//check if aslr is disabled or enabled. helps for actually understanding this stuff.
void debug(vector<string>d){
	for(int i =0;i<d.size();i++){
		cout<<d[i]<<endl;
	}
}

void SaveJump(string address,vector<pair <string,string > > instructions){
	string jumpto = address.substr(address.find(" ")+1);
	stringstream ss;
	ss<<jumpto;
	int line;
	ss>>hex>>line;
	ss.clear();
	vector<string>instructs;

	
	for(auto const &th: instructions){
	
		instructs.push_back(th.first+":"+th.second);
		
	}
	
	
	int point=999999;
	string adi;
	string dumps;

	if(aslr==false){

		adi = address.substr(address.find("jmp 0x40")+9);
	}
	else{
		adi = address.substr(address.find("jmp 0x")+7);
	}
	dumps+="trace "+adi+":\n";

	for(int i =0;i<instructs.size();i++){
		string b = instructs[i].substr(0,instructs[i].find(":"));
		if(b.find(adi)!=string::npos){	
			point=i;
		}
		if(i>=point){	
			dumps+="\n"+instructs[i];
		}
	}

	string addr = address.substr(address.find(" ")+1);
	string filename = "./data/"+addr+".dump";
	ofstream dumper(filename);
	if(dumper.is_open()){
		
		dumper<<dumps;
	}
	dumper.close();
	
}
vector<int> MainOffset(string filename){
	int res =0 ;
	int FOF=0;
	struct stat st;
	stat(filename.c_str(),&st);
	int size = st.st_size;
	int fd = open(filename.c_str(),O_RDONLY);
	char* p = (char*)mmap(0,size,PROT_READ,MAP_PRIVATE,fd,0);
	Elf64_Ehdr *ehdr  = (Elf64_Ehdr*)p ;
	Elf64_Shdr *shdr =  (Elf64_Shdr*)(p+ehdr->e_shoff);
	Elf64_Shdr *shr = &shdr[ehdr->e_shstrndx];
	const char *const shstrp  = p+shr->sh_offset;
	for(int i =0;i<ehdr->e_shnum;i++){
		string n = shstrp+shdr[i].sh_name;;
		if(n == ".text"){

			res = shdr[i].sh_addr;
			FOF = shdr[i].sh_size;
			break;
		
		}
	}
		
	stringstream ss;
	ss<<hex<<res;
	string tete(ss.str());
	cout.clear();
	string ff ="";
	if(tete[0] == '4' && tete[1] == '0'){
		ff+= tete.substr(tete.find("40")+2);
	}

	else{
		ff += tete;
	}
	int lol;
	res=0;
	ff ="0x"+ff;	
	stringstream bb;
	bb<<ff;
	bb>>hex>>lol;
	res = lol;
	vector<int>wow;
	wow.push_back(res);
	wow.push_back(res+FOF);
	return wow;

}
void disas(char *filename){
	stringstream gz;
	unsigned char dat[2];
	stringstream ss;
	vector<string>opcodes;
	vector<pair<string,string> > instructions;
	int cc =0;
	stringstream sb;
	ifstream stream(filename);
	sb<<stream.rdbuf();
	string rawdata(sb.str());
	for(int i =0;i<rawdata.length();i++){
		unsigned char ch= rawdata[i];
		char *bob = (char*)malloc(sizeof(unsigned char)*2);

        sprintf(bob,"%02x",ch);
		opcodes.push_back(bob);
		free(bob);
	}

	vector<int> maininfo = MainOffset(filename);
	int sfh = maininfo[0];
	int mainend = maininfo[1];
	

	cout<<opcodes.size()<<endl;
	
	int memaddrs = 0;
	cout<<"*********************************************************"<<endl;
	for(int i  = 0;i<opcodes.size();i++){
		if(opcodes.size()<=0){
				break;
		}	
		memaddrs = i;
		if(i>=sfh && i<=mainend){
			cout<<opcodes[i]<<":"<<opcodes[i+1]<<endl;
			//mov add sub mul , call , push,pop,stack,pop , jmp ,inc ,find all regs
			string opcode,oprand1,oprand2,fin;
			
		
			if(opcodes[i] == "55"){
				
				opcode = "push";
				oprand1 = "rbp";
				fin=oprand1;	

			}
			if(opcodes[i]== "5d"){
				opcode="pop";
				fin = "rbp";
			}
			if(opcodes[i]== "59"){
				opcode="pop";
				fin = "rcx";
			}
			if(opcodes[i]== "5e"){
				opcode="pop";
				fin = "rsi";
			}
			if(opcodes[i]== "5b"){
				opcode="pop";
				fin = "rbx";
			}
			if(opcodes[i]== "5c"){
				opcode="pop";
				fin = "rsp";
			}
			if(opcodes[i]== "58"){
				opcode="pop";
				fin = "rax";
			}
			if(opcodes[i]== "5f"){
				opcode="pop";
				fin = "rdi";
			}
			if(opcodes[i]== "5a"){
				opcode="pop";
				fin = "rdx";
			}
			if(opcodes[i] =="0f" && opcodes[i+1]== "05"){
				opcode = "syscall";
				fin = " ";

			}
			if(opcodes[i] == "6a"){
				opcode = "push";
				oprand1 = "0x"+opcodes[i+1];
				fin = oprand1;
			}
			if(opcodes[i] == "7e"){
				opcode = "jle";
				ss.clear();
				int re=0;
				ss<<opcodes[i+1];
				ss>>hex>>re;
				re +=sfh;
				ss.clear();
				string lol="";
				ss<<hex<<re;
				ss>>lol;
				oprand1 ="0x"+lol;
				fin = oprand1;
			}
			if(opcodes[i] == "75"){
				opcode = "jne";
				ss.clear();
				int re=0;
				ss<<opcodes[i+1];
				ss>>hex>>re;
				re +=sfh;
				ss.clear();
				string lol="";
				ss<<hex<<re;
				ss>>lol;
				oprand1 ="0x"+lol;
				fin = oprand1;
			}
			if(opcodes[i] == "48" && opcodes[i+1] == "ff"){
				opcode ="inc";
				string mb = opcodes[i+2];
				if(mb == "c0"){
					fin = "rax";

				}
				if(mb == "c1"){
					fin = "rcx";

				}
				if(mb == "c2"){
					fin = "rdx";

				}
				if(mb == "c3"){
					fin = "rbx";

				}
				if(mb == "c4"){
					fin = "rsp";

				}
				if(mb == "c5"){
					fin = "rbp";

				}
				if(mb == "c6"){
					fin = "rsi";

				}
				if(mb == "c7"){
					fin = "rdi";

				}
			

			}
			if(opcodes[i] == "48" && opcodes[i+1] == "01"){
				string combo = opcodes[i+2];
				string mb = opcodes[i+1];
			
				
				opcode ="add";
				
				if(combo == "ec"){
					fin = "rsp, rbp";
				}
				
				if(combo == "c7"){
					fin = "rdi, rax";
				}
				if(combo == "e0"){
					fin = "rax,rsp";
				}
				if(combo == "e8"){
					fin = "rax,rbp";
				}
				if(combo == "c8"){
					fin = "rax,rcx";
				}
				if(combo == "c0"){
					fin = "rax,rax";
				}
				if(combo == "d0"){
					fin = "rax,rdx";
				}

				if(combo == "d8"){
					fin = "rax,rbx";
				}
				if(combo == "f0"){
					fin = "rax,rsi";
				}
				if(combo == "f8"){
					fin = "rax,rdi";
				}

				if(combo == "c7"){
					fin = "rdi,rax";
				}
				if(combo == "fc"){
					fin = "rsp,rdi";
				}	
				
			}
			if(opcodes[i] == "48" && opcodes[i+1] == "29"){
				string combo = opcodes[i+2];
				string mb = opcodes[i+1];
				opcode ="sub";
				
				if(combo == "ec"){
					fin = "rsp, rbp";
				}
				
				if(combo == "c7"){
					fin = "rdi, rax";
				}
				if(combo == "e0"){
					fin = "rax,rsp";
				}
				if(combo == "e8"){
					fin = "rax,rbp";
				}
				if(combo == "c8"){
					fin = "rax,rcx";
				}
				if(combo == "c0"){
					fin = "rax,rax";
				}
				if(combo == "d0"){
					fin = "rax,rdx";
				}

				if(combo == "d8"){
					fin = "rax,rbx";
				}
				if(combo == "f0"){
					fin = "rax,rsi";
				}
				if(combo == "f8"){
					fin = "rax,rdi";
				}

				if(combo == "c7"){
					fin = "rdi,rax";
				}
				if(combo == "fc"){
					fin = "rsp,rdi";
				}	
				
			}
			if(opcodes[i] == "48" && opcodes[i+1] == "83"){
				
				string reg = opcodes[i+2];


				if(reg == "ff"){
					opcode = "cmp";	
					oprand1 ="rdi";
					oprand2 = "0x"+opcodes[i+3];
					fin = oprand1+", "+ oprand2;		
				}
				if(reg == "f8"){
					opcode = "cmp";

					oprand1 ="rax";
					oprand2 = "0x"+opcodes[i+3];
					fin = oprand1+", "+ oprand2;		
				}
				if(reg == "fc"){
					opcode = "cmp";


					oprand1 ="rsp";
					oprand2 = "0x"+opcodes[i+3];
					fin = oprand1+", "+ oprand2;		
				}
				if(reg == "fd"){
					opcode = "cmp";


					oprand1 ="rbp";
					oprand2 = "0x"+opcodes[i+3];
					fin = oprand1+", "+ oprand2;		
				}
				if(reg == "fe"){
					opcode = "cmp";


					oprand1 ="rsi";
					oprand2 = "0x"+opcodes[i+3];
					fin = oprand1+", "+ oprand2;		
				}
				if(reg == "f9"){
					opcode = "cmp";


					oprand1 ="rcx";
					oprand2 = "0x"+opcodes[i+3];
					fin = oprand1+", "+ oprand2;		
				}
				if(reg == "fa"){
					opcode = "cmp";


					oprand1 ="rdx";
					oprand2 = "0x"+opcodes[i+3];
					fin = oprand1+", "+ oprand2;		
				}
				if(reg == "fb"){
					opcode = "cmp";
					oprand1 ="rbx";
					oprand2 = "0x"+opcodes[i+3];
					fin = oprand1+", "+ oprand2;		
				}

				

				//SUB 


				if(reg == "ef"){
					opcode = "sub";	
					oprand1 ="rdi";
					oprand2 = "0x"+opcodes[i+3];
					fin = oprand1+", "+ oprand2;		
				}
				if(reg == "e8"){
					opcode = "sub";

					oprand1 ="rax";
					oprand2 = "0x"+opcodes[i+3];
					fin = oprand1+", "+ oprand2;		
				}
				if(reg == "ec"){
					opcode = "sub";


					oprand1 ="rsp";
					oprand2 = "0x"+opcodes[i+3];
					fin = oprand1+", "+ oprand2;		
				}
				if(reg == "ed"){
					opcode = "sub";


					oprand1 ="rbp";
					oprand2 = "0x"+opcodes[i+3];
					fin = oprand1+", "+ oprand2;		
				}
				if(reg == "ee"){
					opcode = "sub";


					oprand1 ="rsi";
					oprand2 = "0x"+opcodes[i+3];
					fin = oprand1+", "+ oprand2;		
				}
				if(reg == "e9"){
					opcode = "sub";


					oprand1 ="rcx";
					oprand2 = "0x"+opcodes[i+3];
					fin = oprand1+", "+ oprand2;		
				}
				if(reg == "ea"){
					opcode = "sub";


					oprand1 ="rdx";
					oprand2 = "0x"+opcodes[i+3];
					fin = oprand1+", "+ oprand2;		
				}
				if(reg == "eb"){
					opcode = "sub";
					oprand1 ="rbx";
					oprand2 = "0x"+opcodes[i+3];
					fin = oprand1+", "+ oprand2;		
				}


				//add
				if(reg == "c7"){
					opcode = "add";	
					oprand1 ="rdi";
					oprand2 = "0x"+opcodes[i+3];
					fin = oprand1+", "+ oprand2;		
				}
				if(reg == "c0"){
					opcode = "add";

					oprand1 ="rax";
					oprand2 = "0x"+opcodes[i+3];
					fin = oprand1+", "+ oprand2;		
				}
				if(reg == "c4"){
					opcode = "add";


					oprand1 ="rsp";
					oprand2 = "0x"+opcodes[i+3];
					fin = oprand1+", "+ oprand2;		
				}
				if(reg == "c5"){
					opcode = "add";


					oprand1 ="rbp";
					oprand2 = "0x"+opcodes[i+3];
					fin = oprand1+", "+ oprand2;		
				}
				if(reg == "c6"){
					opcode = "add";


					oprand1 ="rsi";
					oprand2 = "0x"+opcodes[i+3];
					fin = oprand1+", "+ oprand2;		
				}
				if(reg == "c1"){
					opcode = "add";


					oprand1 ="rcx";
					oprand2 = "0x"+opcodes[i+3];
					fin = oprand1+", "+ oprand2;		
				}
				if(reg == "c2"){
					opcode = "add";


					oprand1 ="rdx";
					oprand2 = "0x"+opcodes[i+3];
					fin = oprand1+", "+ oprand2;		
				}
				if(reg == "c3"){
					opcode = "add";
					oprand1 ="rbx";
					oprand2 = "0x"+opcodes[i+3];
					fin = oprand1+", "+ oprand2;		
				}
				
			}
			if(opcodes[i] == "48" && opcodes[i+1] == "89"){
			
				string combo = opcodes[i+2];
				string mb = opcodes[i+1];
			
				
				opcode ="mov";
				
				if(combo == "ec"){
					fin = "rsp, rbp";
				}
				if(combo == "c7"){
					fin = "rdi, rax";
				}
				if(combo == "e0"){
					fin = "rax,rsp";
				}
				if(combo == "e8"){
					fin = "rax,rbp";
				}
				if(combo == "c8"){
					fin = "rax,rcx";
				}
				if(combo == "c0"){
					fin = "rax,rax";
				}
				if(combo == "d0"){
					fin = "rax,rdx";
				}
				if(combo == "d8"){
					fin = "rax, rbx";
				}
				
				if(combo == "f0"){
					fin = "rax,rsi";
				}
				if(combo == "f8"){
					fin = "rax,rdi";
				}

				if(combo == "c7"){
					fin = "rdi,rax";
				}
				if(combo == "fc"){
					fin = "rsp,rdi";
				}	
				
				
			}

		
			if(opcodes[i] == "b8"){
				opcode = "mov";
				oprand1 = "rax";
			    oprand2 = "0x"+opcodes[i+1];	
				fin = oprand1+", "+oprand2;
				
			
			}

			if(opcodes[i] == "bf"){
				opcode = "mov";
				oprand1 = "rdi";
			       	oprand2 = "0x"+opcodes[i+1];	
				fin = oprand1+", "+oprand2;

			
			}

			if(opcodes[i] == "b9"){
				opcode = "mov";
				oprand1 = "rcx";
			       	oprand2 = "0x"+opcodes[i+1];	
				fin = oprand1+", "+oprand2;
				
			
			}

			if(opcodes[i] == "bc"){
				opcode = "mov";
				oprand1 = "rsp";
			       	oprand2 = "0x"+opcodes[i+1];	
				fin = oprand1+", "+oprand2;
			
			
			}
			if(opcodes[i] == "bd"){
				opcode = "mov";
				oprand1 = "rbp";
			       	oprand2 = "0x"+opcodes[i+1];	
				fin = oprand1+", "+oprand2;
				
			
			}

			if(opcodes[i] == "ba"){
				opcode = "mov";
				oprand1 = "edx";
			       	oprand2 = "0x"+opcodes[i+1];	
				fin = oprand1+", "+oprand2;
				
			}

			if(opcodes[i] == "bb"){
				opcode = "mov";
				oprand1 = "rbx";
			       	oprand2 = "0x"+opcodes[i+1];	
				fin = oprand1+", "+oprand2;
				
			
			}
			if(opcodes[i] == "be"){
				opcode = "mov";
				oprand1 = "rsi";
			    oprand2 = "0x"+opcodes[i+1];	
				fin = oprand1+", "+oprand2;
				
			
			}	
			if(opcode.length() >0 && fin.length() >0){
				string clp1 = opcode+"\t"+fin;
				string mem = "";
				ss.clear();
				ss<<memaddrs;
				ss>>mem;
				ss.clear();
				instructions.push_back(make_pair(mem,clp1));
				opcode ="";
				fin = "";
				oprand1="";
				oprand2="";
				clp1="";

			}	
			
		}		
	}
	for(auto const& th:instructions){
		cout<<th.first<<"\t"<<th.second<<endl;
	}
	
	cout<<"\ndonna disassembler v3.0 (text-based only)\n ";
	//saving and priting some stuff ngl.
	vector<intptr_t>regs;

	vector<string>jumps;
	cout<<"\nDissassembly of .text section:\n"<<endl;
	string decompiled;
	decompiled+="\nDissassembly of .text section:\n";
	for(auto const &th : instructions){
		if(th.second.compare("mov ,")!=0){

			if(th.second.find("jle")!=string::npos){
				jumps.push_back(th.second);
			}
			if(th.second.find("je")!=string::npos){
				jumps.push_back(th.second);
			}
			if(th.second.find("jmp")!=string::npos){
				jumps.push_back(th.second);
			}
			
			if(th.second.find("jne")!=string::npos){
				jumps.push_back(th.second);	
			}
			
			
			if(th.second != " "){
				stringstream conv;
				string memaddr = th.first;
				memaddr.erase(memaddr.begin());
			
				memaddr = "0x0000555555555"+memaddr;

				ss.clear();
				intptr_t addr;
				string dat = memaddr;
				
				conv<<dat;
				conv>>hex>>addr;
				
				regs.push_back(addr);
				conv.clear();

				cout<<memaddr<<":"<<"\t"<<th.second<<endl;

				decompiled+=memaddr+":"+"\t"+th.second+"\n";
		}

				
			
			
		}
	}

	//dump the jumps
	for(int i =0;i<jumps.size();i++){
		SaveJump(jumps[i],instructions);
	}
	string f=filename;
	f = "/var/tmp/donna/"+f.substr(0,f.find(".c"))+".gpd";
	ofstream writer(f);
	
	writer<<decompiled;
	
	writer.close();

	string fil = filename;
	
    registerdump(regs,fil);
	cout<<"Register data dumped."<<endl;


	
}
int main(int argc, char *argv[]){
	if(argc <2){
		printf("********** donna disassembler man-db **********\n\ti'm not into you, i'm donna :)\nHELP:\n\tIf you want to set a breakpoint,well they are automatically done for you :) \n\tIf you wish to disassemble the file,run donna with filename arg.\n\tExample: ./donna {filename}\n\nhave a nice day\n\t-nitrodegen\n");
    	exit(1);
	}

	string cmd = argv[1];
	if(cmd=="-d" || cmd=="--disassemble" || cmd=="--disas"){
		
		if(argc<2){
			cout<<"Disassembly runs with: -d {filename}"<<endl;
		}

		disas(argv[2]);
		dump(argv[2]);

	}
	
}
