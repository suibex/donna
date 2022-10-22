/*
 *	*part of nitrodegen's god tools for debugging*
 *
 *	donna debugger v4.0 (one that actually works on arm64)
 *		decompiler header.
 *		usage:decompiling the binary and returning instructions along with addresses.
 *	
 *	(C) Gavrilo Palalic 2022.
 */
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
#include <unistd.h>
#include <bitset>
#include <sstream>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <vector>
#include "osx_elf.h"
#define MADDR 0x1234
using namespace std;
vector<string> split(string str,string del){
	vector<string>dele;
	ssize_t beg,pos=0;
	while((beg=str.find_first_not_of(del,pos)) != string::npos){ // loop until you find everything that isn't a delimiter , and always set that to be beginning
		pos = str.find_first_of(del,pos+1);//position is always the next case of del
	
		dele.push_back(str.substr(beg,pos-beg)); // and push 
	}
	return dele;
}

int bin_to_dec(string a){
    
    int res=0;
    int base =1 ;
    for(int i = a.length()-1;i>=0;i--){
        if(a[i] == '1'){
          res+=base;

        }
        base=base*2;

    }
    return res;
}
class Banker{

  public:
    string bank_file;
    int opcode_size;
    vector<string>bank_ocodes;
    vector<string>decoded;
    bool debug;

	//this is where we load the actual instruction bank, split all lines and insert into new vector.   
    Banker(string filename,bool de){
        bank_file=filename;
        debug = de;
        if(debug){
         printf("\n*** loading bank:%s",bank_file.c_str());     
        }
        ifstream stream(bank_file);
        stringstream ss;
       
        ss<<stream.rdbuf();
        string data(ss.str()); 
        bank_ocodes = split(data,"\n");    
        if(debug){
          printf("\n*** bank loaded.");
        }
    } 
    //find if any of the binary nums contain 1 or 0 is it usable?
    int calculate_occurance(string a){
        int b=0;
        for(int i =0;i<4;i++){
            if(a[i] == '0' || a[i] == '1'){
                b++;
            }
        }
        return b;
    }
    //find if any of the chars of a string match the b chars at the same pos
    int find_binary(string a, string b){
        
        int res = 0; 
        for(int i  =0;i<4;i++){
            if(a[i] == '0' || a[i] == '1'){
                if(a[i] == b[i]){
                    res++;
                 }      
              }
        }
        return res;

    } //searching if the instructions of the opcoded number match with some in the database
    string search_for_instruction(string o1,string o2,string o3,string o4,string o5,string o6){
	    vector<string>results;

        for(int i =2;i<bank_ocodes.size();i++){
            string g = bank_ocodes[i];
            vector<string>spt = split(g,"-");
            
            string tmplt = spt[0];
            string blob;
            for(int j =0;j<tmplt.length();j++){
              if(tmplt[j] != ' '){
                blob+=tmplt[j];
              }       
           
           }
               string op1,op2;
                for(int j =0;j<4;j++){
                  op1+=blob[j];
                }
                for(int j=4;j<8;j++){
                    op2+=blob[j];
                }
                string op3;
                for(int j =8;j<12;j++){
                    op3+=blob[j];
                }
                string op4;
                for(int j =28;j<32;j++){
                    op4+=blob[j];
                }
          	string op5;
                for(int j =20;j<24;j++){
                    op5+=blob[j];
                }
		string op6;
                for(int j =24;j<28;j++){
                    op6+=blob[j];
                }
           if(o2== op2){
            //xx00 1110 001m
              //cout<<o1<<":"<<o2<<":"<<o3<<endl;


              int occurance_op1= calculate_occurance(op1);
              int occurance_op3= calculate_occurance(op3);

              int occurance_op4= calculate_occurance(op4);
              int occurance_op5= calculate_occurance(op5);
   	     int occurance_op6= calculate_occurance(op6);


             //cout<<occurance_op1<<":"<<occurance_op3<<endl;
   //getting first 4 binary nums and comparing with opcoded ones.
   	      int found_1 = find_binary(op1,o1);
              int found_3 = find_binary(op3,o3);
              int found_4 = find_binary(op4,o4);
	      int found_5 = find_binary(op5,o5);
 	      int found_6 = find_binary(op6,o6);     
              if(debug){
                
   
                cout<<found_1<<":"<<found_3<<endl;
                cout<<g<<"\t"<<"1: "<<op1<<":"<<o1<< "|\t"<<found_1<<" should find:" <<occurance_op1<<endl;
                cout<<"3: "<<op3<<":"<<o3<< "|\t"<< found_3<<" should find:" <<occurance_op3<<endl;
   		 cout<<"4: "<<op4<<":"<<o4<< "|\t"<< found_4<<" should find:" <<occurance_op4<<endl;
   		 cout<<"5: "<<op5<<":"<<o5<< "|\t"<< found_5<<" should find:" <<occurance_op5<<endl;
   		 cout<<"6: "<<op6<<":"<<o6<< "|\t"<< found_6<<" should find:" <<occurance_op6<<endl;
   			               

              }
              if(found_4 >= occurance_op4 && found_5 >= occurance_op5 &&  found_6 >= occurance_op6 && found_1 >= occurance_op1 && found_3>=occurance_op3){
                return g;
              }
	      

            }
       }
       
       return "NULL";
    }
    char define_oprand(string oprand){
          char result;
          //took from binutils
          if(oprand != "EXCEPTION" && oprand.size() == 2 ){
             result=oprand[1];
             
          }        
          else{
            if(oprand.find("ADDR") != string::npos && oprand != "ADDR_SIMM9" && oprand != "ADDR_SIMPLE"){
                 result='i';
            }
            else if(oprand == "IMMR"){
                result='i';
            }
            else if(oprand == "IMMS"){
                result='i';
            }
            else if(oprand == "CCMP_IMM" || oprand == "NZCV" || oprand=="COND"){
              result='c';
            }
            else if(oprand == "UIMM3_OP2" ){
                result='o';
            }
            else if(oprand == "HALF"){
              result='i';
            }
	    else if(oprand == "ADDR_SIMM7"){
		result='t';
	    }		
            else if(oprand =="AIMM"){
              result='i';
            }   
            if(oprand == "EXCEPTION"){
              result='i';
            }
            else if(oprand == "Rd_SP"){
              result='d';
            }
            else if(oprand == "Rn_SP" || oprand == "Rm_EXT"){
              result='n';
            }

          }//TODO: FIGURE OUT WHY ARE THERE DIFFERENT LETTERS TO THIS!
          return result;
    }
    string opcode_decode(int number){
      string res = bitset<32>(number).to_string();
      if(debug){
         cout<<"\nbinary :"<<res<<endl;
      }
      //vector<int>result;
      string op1,op2;
      for(int i =0;i<4;i++){
          op1+=res[i];
      }
      for(int i =4;i<8;i++){
          op2+=res[i];
      }
      string op3;
      for(int i =8;i<12;i++){
          op3+=res[i];
      }

      string op4;
      for(int i =28;i<32;i++){
          op4+=res[i];
      }

      string op5;
      for(int i =20;i<24;i++){
          op5+=res[i];
      }
      string op6;
      for(int i =24;i<28;i++){
          op6+=res[i];
      }

      string blob=search_for_instruction(op1,op2,op3,op4,op5,op6);
      if(blob== "NULL"){
          return "<not defined>";
          printf("\ncannot decode instruction.");
          //exit(1);
      }
      
     
      if(debug){
         cout<<"*** binary:"<<res<<endl;
      
        cout<<"\n******************************************************\nFound matching instruction:"<<blob<<endl;
      }
      vector<string>popit = split(blob,"-");
      
      string scheme = popit[0];
      string he;
      for(int j=0;j<scheme.length();j++){
          if(scheme[j] != ' '){
            he+=scheme[j];
          }
      }
      scheme =he;
      
      vector<string>instr_package;

      vector<string>extension = split(popit[1]," ");
      vector<char>look_for;
      extension.erase(extension.begin());
      if(debug){
        cout<<"extension size:"<<extension.size()<<endl;
      }
      instr_package.push_back(extension[0]); 
      
      look_for.push_back(define_oprand(extension[1]));
      look_for.push_back(define_oprand(extension[2]));
      if(extension.size() ==4){
        look_for.push_back(define_oprand(extension[3]));
      }
      stringstream ss;
      
      for(int i =0;i<look_for.size();i++){
          string op;
          for(int j =0;j<scheme.length();j++){
              if(scheme[j] == look_for[i]){
                  op+=res[j];
              }
          } 
          
          int num = bin_to_dec(op);
          ss.clear();
          ss<<num;
          string n;
          ss>>n;
           
          if(look_for[i] == 'd'  || look_for[i] =='t' || look_for[i] == 'n'){
            op = "x";
          }

          else{
            op="#";
          }
          op = op+n;
          instr_package.push_back(op);

      }
      string instruction=instr_package[0]+"\t";
      for(int i =1;i<instr_package.size();i++){
        if(i < instr_package.size()-1){
          instruction+=instr_package[i]+",";
          }
         else{
         instruction+=instr_package[i];
         }
      }
      //cout<<instruction;
     return instruction;
    }
    vector<pair<unsigned int,unsigned int> >banker_load_file(string f){

       vector<pair<unsigned int,unsigned int>> instructions;
        //load ELF file here.
        int fd = open(f.c_str(),O_RDONLY);
        struct stat st;
        fstat(fd,&st);
        long size = st.st_size;

        if(debug){
          cout<<"\n*** file size:"<<size<<endl;
        }
        unsigned char *mb = (unsigned char*)mmap(0,size,PROT_READ,MAP_PRIVATE,fd,0);
        
        Elf64_Ehdr *ehdr  = (Elf64_Ehdr*)mb;
        int start_of_program=0;
        int size_of_executable=0;
        int runtime_addr=0;
        
        int arch = ehdr->e_ident[4];
        if(arch == 2){
        Elf64_Shdr *shdr =  (Elf64_Shdr*)(mb+ehdr->e_shoff);
        Elf64_Shdr *shr = &shdr[ehdr->e_shstrndx];
        Elf64_Phdr *phdr  = (Elf64_Phdr*)(mb+ehdr->e_phoff); 
       
        const char *const shstrp  = (const char*)mb+shr->sh_offset;
        for(int i =0;i<ehdr->e_shnum;i++){
              string n = shstrp+shdr[i].sh_name;
              if(n == ".text"){
                  start_of_program=shdr[i].sh_addr;
                  size_of_executable=shdr[i].sh_size;
                  break;
              }
               //  }
              
        }
        runtime_addr =phdr[0].p_vaddr;

        }
        else{
          Elf32_Ehdr *ehdr1 = (Elf32_Ehdr*)mb;
          Elf32_Shdr *shdr =  (Elf32_Shdr*)(mb+ehdr1->e_shoff);
          
          Elf32_Shdr *shr = &shdr[ehdr1->e_shstrndx];
          Elf32_Phdr *phdr  = (Elf32_Phdr*)(mb+ehdr1->e_phoff); 
          const char *const shstrp  = (const char*)mb+shr->sh_offset;
          for(int i =0;i<ehdr1->e_shnum;i++){
                string n = shstrp+shdr[i].sh_name;
                if(n == ".text"){
                    start_of_program=shdr[i].sh_addr;
                    size_of_executable=shdr[i].sh_size;
                    break;
                }
                 //  }
                
          }
            runtime_addr =phdr[0].p_vaddr;
 
        }
        //how to find this real address in file? actualaddr - runtime address
      
        //it should always be the first one. lmao.
        int actual_entry= start_of_program-runtime_addr;
       
        if(debug){
          printf("\n***.text entry:0x%lx , runtime address:0x%lx",start_of_program,runtime_addr);
          printf("\n*** reading .text entry contents...");
        }
        
        stringstream ss;
        ss.clear();
	int bobi =0; 
        for(int i =actual_entry;i<actual_entry+size_of_executable;i+=4){
          string num;
          int k =0;
          for(int j =i+3;j>=i;j--){
              ss.clear();
              char dd[32];
              sprintf(dd,"%x",mb[j]);
              
              string n;
              ss<<dd;
              ss>>n;
              if(n.length() == 1){
                n="0"+n;
              }
              
              num+=n;
          }
          //cout<<num<<endl;
       
           ss.clear();
             
             unsigned int  kk=0;
             ss<<hex<<num;
             ss>>kk;
             //cout<<kk<<endl;
              
		unsigned int address=start_of_program+bobi;
		bobi+=4;
		 
   		instructions.push_back(make_pair(address,kk));
        }
        
        if(debug){
          printf("\n*** file read. loaded instructions:%d",instructions.size());
          }
          
          return instructions;   
    }
};
/*
  
  structure of bank file
  filename:
  opcode_size,
  instruction->oprand->opcode
  
*/
/****** example usage of load_bank. decoding!*/
 // int main(int argc,char* argv[]){
 /* 
 
  Banker *bank = new Banker("./banks/arm64.bank",false);
  vector<pair<unsigned int ,unsigned int>> instr =   bank->banker_load_file("./testing/test_exec");
  for(int i =0;i<instr.size();i++){
 	int addr = instr[i].first;
	int exec = instr[i].second;

   	string res = bank->opcode_decode(exec);
	printf("0x%x\t: %s\n",addr,res.c_str());	 
  }
	*/
//}

