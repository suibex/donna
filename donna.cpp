/*
*******************************************************************************************************************************************

		donna disassembler v1.1
			-main module

		Last update commited at: Mar 21 22.

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
#include <random>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
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
void disas(char *filename){
	FILE *a;
	a = fopen(filename,"r");
	unsigned char dat[2];
	stringstream ss;
	vector<string>opcodes;

	vector<pair<string,string> > instructions;


	fseek(a,0,SEEK_SET);
	while(true){
		int re = fread(dat,sizeof(unsigned char),1,a);
		if(re<=0){
			break;

		}
		char *b = (char*)malloc(100+sizeof(unsigned char));
		
		sprintf(b,"\n%02x\n",dat[0]);
		string t;
		ss<<b;	
		ss>>t;
		ss.clear();
		opcodes.push_back(t);


	}
	//when ASLR is used , you have to find 48 8d 3d and then the number you are adding to the already existing line +7 , and that is main start.
	//when no aslr is used , you just find 48,c7 and 6 bits for address of start.
	//_start is always 45 chars long.
	//first find the _start , then find the stupid xor , then see where it leads :)
	int i =opcodes.size();
	int _startfunc =0;
	string mainHeader;
	while(true){
		
		if(opcodes[i] == "31" && opcodes[i+1] == "ed") {
			cout<<"\t_start at:"<<"0x"<<hex<<i<<endl;
			_startfunc=i;//4 out of 45 
			break;
		}
		i--;
	}
	
	int len=45;

	//check if aslr or not 
	//if 21 line is mov and not lea we god false , else it is true
	int check = _startfunc+20;

	if(opcodes[check]=="48" && opcodes[check+1] =="8d"){
		aslr=true;
		

	}
	else if(opcodes[check] == "48" && opcodes[check+1]=="c7"){
		aslr=false;
		
	}
	if(aslr == 1){
		cout<<"\tASLR status:enabled"<<endl;
	}
	else{
		cout<<"\tASLR status:disabled"<<endl;
	}
	
	if(aslr == true){
		stringstream ss;

		string toadd = opcodes[check+3];
		ss<<toadd;
		int g;
		ss>>hex>>g;
		// 00 00 00 1 = 2 + 2 + 2 + 1 = 7
		g+=7;
		ss.clear();
		int fi = g+check;
		ss<<fi;
		string calculated;
		ss>>calculated;
		ss.clear();
		
		mainHeader=calculated;
		cout<<"\tmain at:"<<"0x"<<mainHeader<<endl;
	}
	else{
		// example : 0x401190 was in hex = 90 11 40 so just reversing it , you get the address :)
		mainHeader=opcodes[check+4]+opcodes[check+3];
		cout<<"\tmain at:"<<"0x40"<<mainHeader<<endl;
	}
	
	//idk why i chose reverse , but i mean every hex i literally just reversed shit.
	stringstream gz;
	gz<<mainHeader;
	int sfh;
	gz>>hex>>sfh;
	gz.clear();
	i = 0;
	int memaddrs=0;
	int mainend=0;
	for(int j  = 0;j<opcodes.size();j++){
		if(opcodes.size()<=0){
				break;
		}	
		memaddrs=j;
		if(j>=sfh){
			if(opcodes[j]=="c3"){
				gz.clear();
				
			
				gz<<hex<<memaddrs;
				string l;
				gz>>l;
				
				mainend=memaddrs;
				break;
				
			}
		}
	}
	if(aslr == true){
		cout<<"\tmain ends at:"<<"0x"<<hex<<mainend<<endl;
	}
	else{
		cout<<"\tmain ends at:"<<"0x40"<<hex<<mainend<<endl;
	}	
	cout<<"*********************************************************"<<endl;
	for(int j  = 0;j<opcodes.size();j++){
		if(opcodes.size()<=0){
				break;
		}	
		memaddrs=j;
		if(j>=sfh && j<=mainend){
			//bf 30 00 00 00	mov    edi,0x30
			//c7 45 f0 00 00 00 00	mov    DWORD PTR [rbp-0x10],0x0
			//c7 45 f4 05 00 00 00	mov    DWORD PTR [rbp-0xc],0x5
			//c7 45 fc 05 00 00 00	mov    DWORD PTR [rbp-0x4],0x5
			
			
			if(opcodes[j] =="bf"){
				gz.clear();
				gz<<hex<<memaddrs;
				string l;
				gz>>l;
				string f,s,call;
				call="mov";
				if(opcodes[j+2] == "00" && opcodes[j+4]== "00"){
					
					//well edi burazer
					f = "\tedi";
					s = ",0x"+opcodes[j+1];
					
				}
				string comb = call+f+s;
				instructions.push_back(make_pair(l,comb));
				
			}
			else if(opcodes[j] == "e8"){
				gz.clear();
				gz<<hex<<memaddrs;
				string l;
				gz>>l;
				gz.clear();
				string f,call;
				call="call";
				if(opcodes[j+1] == "bc"){
					f="\t<printf@plt>";
				}
				else if(opcodes[j+1] == "91"){
					f="\t<printf@plt>";
				}
				else if(opcodes[j+1] == "77"){
					f="\t<printf@plt>";
				}
				else if(opcodes[j+1] == "47"){
					f="\t<putchar@plt>";
				}
				else if(opcodes[j+1] == "51"){
					f="\t<printf@plt>";
				}
				else if(opcodes[j+1] == "c4"){
					f="\t<printf@plt>";
				}
				else if(opcodes[j+1] == "da"){
					f="\t<printf@plt>";
				}
				else{
					f="\t<function@{at address}>";
				}
				
				//4470 //4660 / 190 lines diffrence -26 diff / WHAT THE FUCK???





				string comb = call+f;
				instructions.push_back(make_pair(l,comb));
			}
			else if(opcodes[j] == "eb" && opcodes[j+1] != "fe" && opcodes[j+1] != "ff" && opcodes[j+2] != "ff"){
				//jmp 
				gz.clear();
				gz<<hex<<memaddrs;
				string l;
				gz>>l;
				gz.clear();
				string f,call;
		
				//eb 18	jmp    0x4011c8 <main+56>
				stringstream lol;
				lol<<opcodes[j+1];
				int zozi;
				lol>>hex>>zozi;
				lol.clear();

				int zfl = zozi+memaddrs+2;
				lol<<hex<<zfl;
				lol>>f;
				if(aslr == true){
					f = "\t0x"+f;
				}
				else{
					f = "\t0x40"+f;
				}
			
	
				call="jmp";
				
				string comb = call+f;
				instructions.push_back(make_pair(l,comb));
			}
			else if(opcodes[j] == "7e" && opcodes[j+1] != "fe" && opcodes[j+1] != "ff" && opcodes[j+2] != "ff" ){
				//jmp 
				gz.clear();
				gz<<hex<<memaddrs;
				string l;
				gz>>l;
				gz.clear();
				string f,call;
		
				//eb 18	jmp    0x4011c8 <main+56>
				stringstream lol;
				lol<<opcodes[j+1];
				int zozi;
				lol>>hex>>zozi;
				lol.clear();

				int zfl = zozi+memaddrs+2;
				lol<<hex<<zfl;
				lol>>f;
				if(aslr == true){
					f = "\t0x"+f;
				}
				else{
					f = "\t0x40"+f;
				}
			
	
				call="jle";
				
				string comb = call+f;
				instructions.push_back(make_pair(l,comb));
			}

			//add addl instruction and cmp 
			/*
				Data:					83 45  01      f0 
 				83 45 f0 01          	addl   $0x1,-0x10(%rbp)
  				83 7d f0 09          	cmpl   $0x9,-0x10(%rbp)
				83 7d f4 05          	cmpl   -0xc(%rbp) , 0x5
				83 7d f4 00          	cmpl   $0x0,-0xc(%rbp)
				83 7d f4 04          	cmpl   $0x4,-0xc(%rbp)
				83 7d f0 09          	cmpl   $0x9,-0x10(%rbp)

				CMP 
				3b 45 fc             	cmp    %eax,-0x4(%rbp)

				another mov
				8b 45 f8             	mov    %eax,-0x8(%rbp)

			*/
		
			else if(opcodes[j]=="3b" && opcodes[j+1] == "45"){
				//woho ,eax 
				gz.clear();
				gz<<hex<<memaddrs;
				string l;
				gz>>l;
				gz.clear();
				string f,s,call;
				f="\teax";
				if(opcodes[j+2].find("f") != string::npos){
						string num = opcodes[j+2].substr(opcodes[j+2].find("f")+1);
						if(num == "0"){
							s = ",dword[rbp-0x10]";
						}
						else if(num == "4"){
							s = ",dword[rbp-0xc]";
						}
						else if(num == "8"){
							s = ",dword[rbp-0x8]";
						}
						else{
							s = ",dword[rbp-(offset:0x"+num+")]";	
						}
					}
					else{
							s = ",dword[rbp-(offset:"+opcodes[j+2]+")]";	
					}
				
				call="cmp";
				
				string comb = call+f+s;
				instructions.push_back(make_pair(l,comb));
			}
			else if(opcodes[j] == "83" && opcodes[j+1] == "45" || opcodes[j+1] == "7d"){
				gz.clear();
				gz<<hex<<memaddrs;
				string l;
				gz>>l;
				gz.clear();
				string f,s,call;
				if(opcodes[j+1]=="45"){
					//addl
					call="add";
					s = ",0x"+opcodes[j+3];
					if(opcodes[j+2].find("f") != string::npos){
						string num = opcodes[j+2].substr(opcodes[j+2].find("f")+1);
						if(num == "0"){
							f = "\tdword[rbp-0x10]";
						}
						else if(num == "4"){
							f = "\tdword[rbp-0xc]";
						}
						else{
							f = "\tdword[rbp-(offset:0x"+num+")]";	
						}
					}
					else{
							f = "\tdword[rbp-(offset:"+opcodes[j+2]+")]";	
					}
				}
				if(opcodes[j+1]=="7d"){
					//cmp
					call="cmpl";
					if(opcodes[j+2].find("f") != string::npos){
						string num = opcodes[j+2].substr(opcodes[j+2].find("f")+1);
						if(num == "0"){
							f = "\tdword[rbp-0x10]";
						}
						else if(num == "4"){
							f = "\tdword[rbp-0xc]";
						}
						else{
							f = "\tdword[rbp-(offset:0x"+num+")]";	
						}
					}
					else{
							f = "\tdword[rbp-(offset:"+opcodes[j+2]+")]";	
					}
					s = ",0x"+opcodes[j+3];
				



				}
				string comb = call+f+s;
				instructions.push_back(make_pair(l,comb));
			}
			else if(opcodes[j] == "74" && opcodes[j+1] != "fe" && opcodes[j+1] != "ff" && opcodes[j+2] != "ff" ){
				//jmp 
				gz.clear();
				gz<<hex<<memaddrs;
				string l;
				gz>>l;
				gz.clear();
				string f,call;
		
				//eb 18	jmp    0x4011c8 <main+56>
				stringstream lol;
				lol<<opcodes[j+1];
				int zozi;
				lol>>hex>>zozi;
				lol.clear();

				int zfl = zozi+memaddrs+2;
				lol<<hex<<zfl;
				lol>>f;
				if(aslr == true){
					f = "\t0x"+f;
				}
				else{
					f = "\t0x40"+f;
				}
			
	
				call="je";
				
				string comb = call+f;
				instructions.push_back(make_pair(l,comb));
			}
			else if(opcodes[j] == "75" && opcodes[j+1] != "fe" && opcodes[j+1] != "ff" && opcodes[j+2] != "ff"){
				//jmp 
				gz.clear();
				gz<<hex<<memaddrs;
				string l;
				gz>>l;
				gz.clear();
				string f,call;
		
				//eb 18	jmp    0x4011c8 <main+56>
				stringstream lol;
				lol<<opcodes[j+1];
				int zozi;
				lol>>hex>>zozi;
				lol.clear();

				int zfl = zozi+memaddrs+2;
				lol<<hex<<zfl;
				lol>>f;
				if(aslr == true){
					f = "\t0x"+f;
				}
				else{
					f = "\t0x40"+f;
				}
			
	
				call="jne";
				
				string comb = call+f;
				instructions.push_back(make_pair(l,comb));
			}
			else if(opcodes[j]=="55"){
				gz.clear();
				gz<<hex<<memaddrs;
				string l;
				gz>>l;
			
				instructions.push_back(make_pair(l,"push rbp"));
				
			}
			else if(opcodes[j]=="5d"){
				gz.clear();
				gz<<hex<<memaddrs;
				string l;
				gz>>l;
			
				instructions.push_back(make_pair(l,"pop rbp"));
				
			}
			else if(opcodes[j]=="c7" && opcodes[j+1] == "45"){
				//DWORD 
				gz.clear();
				gz<<hex<<memaddrs;
				string l;
				gz>>l;
				string f,s,call;
				call = "mov";
			
				f="\tdword ptr[rbp-(offset:0x"+opcodes[j+2]+")]";
				s = ",0x"+opcodes[j+3];

				
				string comb = call+f+s;
				instructions.push_back(make_pair(l,comb));
			}
			else if(opcodes[j]=="c9"){
				gz.clear();
				
			
				gz<<hex<<memaddrs;
				string l;
				gz>>l;
				
				instructions.push_back(make_pair(l,"leave"));
				

			}
			else if(opcodes[j]=="b8"){
				gz.clear();
				gz<<hex<<memaddrs;
				string l;
				gz>>l;
				if(opcodes[j+1]=="00" && opcodes[j+2] == "00"){
					instructions.push_back(make_pair(l,"mov\teax,0x0"));
				}
			
			}
			
	

			else if(opcodes[j]=="48"){
				gz.clear();
				gz<<hex<<memaddrs;
				string l;
				gz>>l;
				string f,s,call;

				/*
						lea is only 48 left
						0  1  2  3  4  5  6
						48 8d 05 5f 0e 00 00	lea    rax,[rip+0xe5f]  
						48 8d 05 39 0e 00 00	lea    rax,[rip+0xe39] 
						48 8d 05 2a 0e 00 00	lea    rax,[rip+0xe2a] 
						48 8d 05 07 0e 00 00	lea    rax,[rip+0xe07] 
						
						1-moving regs
						2- rax moving 
						3-rip moving
						4,5 - values :)

				*/
				if(opcodes[j+1]=="8d" && opcodes[j+6]=="00" && opcodes[j+5]=="00" ){
						//lea definitely 
						//second is rip 
						call="lea";
						if(opcodes[j+2] == "05"){
							//rax is first
							f="\trax";
							string h = opcodes[j+4].substr(opcodes[j+4].find("0")+1)+opcodes[j+3];
							s=",[rip+0"+h+"]";

						}
				}
				
				if(opcodes[j+1] == "89"){
					// movin between registers
					call="mov";
					if(opcodes[j+2]=="e5"){
						f = "\trbp";
						s=",rsp";
					}
					if(opcodes[j+2]=="c7"){
						//mov    rdi,rax
						f="\trdi";
						s = ",rax";

					}
					if(opcodes[j+2]=="45"){
						//mov    rdi,rax
						if(opcodes[j+3].find("f") != string::npos){
							string num = opcodes[j+3].substr(opcodes[j+3].find("f")+1);
						
					
							f ="\tqword[rbp-0x"+num+"]";
						}
						else{
							f="\t0x"+opcodes[i+3];
						}
						s = ",rax";
						

					}
					

				}
				if(opcodes[j+1]=="83"){
					call="sub";
					if(opcodes[j+2]=="ec"){
						// we are talking about rsp sub , so it could only be a number (probably bits that are subbed from memory :)  ) 	
							f="\trsp";
							s=",0x"+opcodes[j+3];
					}
					
				}
					
				
				string comb = call+f+s;
				instructions.push_back(make_pair(l,comb));
				
			}
			else if(opcodes[j]=="c3"){
				gz.clear();
				
			
				gz<<hex<<memaddrs;
				string l;
				gz>>l;
				
				instructions.push_back(make_pair(l,"ret"));
				
			}

			
		}
	}

	cout<<"\ndonna disassembler v1.1 (text-based only)\n ";
	//saving and priting some stuff ngl.
	vector<string>regs;

	vector<string>jumps;
	cout<<"\nDissassembly of 'main' function:\n"<<endl;
	string decompiled;
	decompiled+="\nDissassembly of 'main' function:\n";
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
			
			if(aslr==true){
				
				stringstream conv;
				string memaddr = th.first;
				memaddr.erase(memaddr.begin());
			
				memaddr = "0x0000555555555"+memaddr;
				regs.push_back(memaddr);
				conv.clear();
				cout<<memaddr<<":"<<"\t"<<th.second<<endl;

				decompiled+=memaddr+":"+"\t"+th.second+"\n";
			}
			else{
				string memaddr = "0x000000000040"+th.first;
				regs.push_back(memaddr);
				cout<<"0x000000000040"<<th.first<<":"<<"\t"<<th.second<<endl;

				decompiled+="0x000000000040"+th.first+":"+"\t"+th.second+"\n";
			}
			
		}
	}
	
	//dump the jumps
	for(int i =0;i<jumps.size();i++){
		SaveJump(jumps[i],instructions);
	}
	string f=filename;
	f = "./data/"+f.substr(0,f.find(".c"))+".gpd";
	ofstream writer(f);
	if(writer.is_open()){
		writer<<decompiled;
	}
	else{
		cout<<"Could not open ofstream"<<endl;
		exit(1);
	}
	writer.close();


	for(int i =0;i<regs.size();i++){
		intptr_t addr;
		string dat = regs[i];
		stringstream conv;
		conv<<dat;
		conv>>hex>>addr;
		string fil = filename;
		registerdump(addr,fil);
		
		
	}
	cout<<"Register data dumped."<<endl;
	


}
int main(int argc, char *argv[]){
	if(argc <2){
		printf("********** donna disassembler man-db **********\n\ti'm not into you, i'm donna :)\nHELP:\n\tIf you want to set a breakpoint,well they are automatically done for you :) \n\tIf you wish to disassemble the file,run donna with -d arg.\n\tExample: ./donna -d {filename}\n\nhave a nice day\n\t-nitrodegen\n");
		exit(1);

	}

	string cmd = argv[1];
	if(cmd=="-d" || cmd=="--disassemble" || cmd=="--disas"){
		
		if(argc<2){
			cout<<"Disassembly runs with: -d {filename}"<<endl;
		}
		cout<<"****************** donna ELF Config *********************"<<endl;
		cout<<"ELF File Data:"<<endl;
		disas(argv[2]);

		dump(argv[2]);

	}
	
}
