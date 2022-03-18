/*
*******************************************************************************************************************************************

		donna disassembler v1.1
			-dump module

		Last update commited at: Mar 17 22.

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
#include <unistd.h>
using namespace std;


vector<string>split(string s,int sp){
	int NumSubstrings = s.length() / sp;
	std::vector<std::string> ret;

	for (auto i = 0; i < NumSubstrings; i++)
	{
			ret.push_back(s.substr(i * sp, sp));
	}

	// If there are leftover characters, create a shorter item at the end.
	if (s.length() % sp != 0)
	{
			ret.push_back(s.substr(sp * NumSubstrings));
	}
	return ret;
}
void dump(string file){
	FILE *a;
	a = fopen(file.c_str(),"r");
	unsigned char dat[2];
	stringstream ss;
	fseek(a,0,SEEK_SET);
	string dumped;
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
		dumped+=t;
	}
	vector<string>hexi = split(dumped,10);
	ofstream hexdump("./data/hex.hex");
	for(int i=0;i<hexi.size();i++){
		stringstream ss;
		ss<<hex<<i;
		string oc;
		ss>>oc;
		ss.clear();
		hexdump<<oc<<":"<<hexi[i]<<"\n";
	}
	hexdump.close();


}