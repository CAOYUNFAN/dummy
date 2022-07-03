#include<bits/stdc++.h>
using namespace std;
typedef long long LL;
char filename_input[256];
char filename_output[256];
const char * key_name="key.txt";
void work(FILE * input,FILE * key,FILE * output,int direction);

int main(int argc,char * argv[]){
//	freopen(".in","r",stdin);
//	freopen(".out","w",stdout);
	if(argc!=4||( strcmp(argv[2],"encode")!=0 && strcmp(argv[2],"decode")!=0 )){
		fprintf(stderr,"usage: %s [input file] [direction] [output file]\n",argv[0]);
		fprintf(stderr,"[direcion]: (encode|decode)\nencode for encoding,decode for decoding\n");
		return 1;
	}
	strcpy(filename_input,argv[1]);
	strcpy(filename_output,argv[3]);
	FILE * input=fopen(filename_input,"rb");
	if(!input){
		fprintf(stderr,"Input file %s do not exist. Did you input a wrong name or path?\n",filename_input);
		return 1;
	}
	FILE * key=fopen(key_name,"rb");
	if(!key){
		fprintf(stderr,"File \"key.txt\" do not exist. It should always be at the same path of the program, or the program cannot run correctly.\n");
		fclose(input);
		return 1;
	}
	FILE * output=fopen(filename_output,"wb");
	if(!output){
		fprintf(stderr,"Output file %s cannot be created!\n",filename_output);
		fclose(input);fclose(key);
		return 1;
	}
	printf("working...\n");
	work(input,key,output,strcmp(argv[2],"encode")==0?0:1);
	fclose(input);fclose(key);fclose(output);
	printf("done!\n");
	return 0;
}

const int N=4096;
int key_offset=0,key_total=0;
unsigned char buffer[N],key_buff[N];

inline unsigned char get_nxt(FILE * key){
	if(key_offset<key_total) return key_buff[key_offset++];
	key_offset=0;
	key_total=fread(key_buff,1,N,key);
	if(!key_total){
		fseek(key,0,SEEK_SET);
		key_total=fread(key_buff,1,N,key);	
	}
	return key_buff[key_offset++];
}

void work(FILE * input,FILE * key,FILE * output,int direction){
	int num=N;
	while(num==N){
		num=fread(buffer,1,N,input);
		for(int i=0;i<num;i++){
			if(direction) buffer[i]+=get_nxt(key);
			else buffer[i]-=get_nxt(key);
		}
		fwrite(buffer,1,num,output);
	}
}
