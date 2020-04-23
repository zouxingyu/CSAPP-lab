#include "cachelab.h"
#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include<stdlib.h>
#define MAX_READ 50
typedef struct Line{
	bool valid;
	int last;
	long long tag;
}Line;
long long getmask(int set_bits,int block_bits){
	long long mask=1;
	while(--set_bits>0){
		mask=(mask<<1)+1;
	}
	mask=mask<<block_bits;
	return mask;
}
int pow_two(int set_bits){
	int ret=1;
	while(set_bits-->0){
		ret*=2;		
	}
	return ret;
}
char buf[MAX_READ];
int main(int argc,char **argv){
	int set_bits,lines_per_set,block_bits;
	bool verbose=false;
	char file_name[MAX_READ];
    for(int i=0;i<argc;i++){
    	if(!strcmp(argv[i],"-v")){
    		verbose=true;
    	}else if(!strcmp(argv[i],"-s")){
    		sscanf(argv[i+1],"%d",&set_bits);
    	}else if(!strcmp(argv[i],"-E")){
    		sscanf(argv[i+1],"%d",&lines_per_set);
    	}else if(!strcmp(argv[i],"-b")){
    		sscanf(argv[i+1],"%d",&block_bits);
    	}else if(!strcmp(argv[i],"-t")){
    		sscanf(argv[i+1],"%s",file_name);
    	}
    }
	int size=pow_two(set_bits)*lines_per_set*sizeof(Line);
    Line *cache=(Line*)malloc(size);
	memset(cache,0,size);
    FILE *file;
	file = fopen(file_name, "r");
	long long mask=getmask(set_bits,block_bits);
	char op;
	long long add;
	int tik=0;
	int hit=0,miss=0,eviction=0;
	while(fgets(buf,MAX_READ,file)){
		if(buf[0]=='I'){
			continue;
		}else{
			//printf("%s\n",buf);
			sscanf(buf," %c %llx",&op,&add);
			int set=(add&mask)>>block_bits;
			long long tag=add>>(set_bits+block_bits);
			int start=set*lines_per_set,end=start+lines_per_set,i=start,lru=start;

			while(i<end){
				if(cache[i].tag==tag||cache[i].valid==false){
					break;
				}else{
					lru=cache[i].last<cache[lru].last?i:lru;
				}
				i++;
			}
			if(i==end){
				if(verbose){
					printf("%s",buf);
					switch(op){
					case 'L':
						printf(" miss eviction\n");
						cache[lru].tag=tag;
						cache[lru].last=tik++;
						miss++;
						eviction++;
						break;
					case 'S':
						printf(" miss eviction\n");
						cache[lru].tag=tag;
						cache[lru].last=tik++;
						miss++;
						eviction++;
						break;
					case 'M':
						printf(" miss eviction hit\n");
						cache[lru].tag=tag;
						cache[lru].last=tik++;
						hit++;
						miss++;
						eviction++;
					}
				}else{
					switch(op){
					case 'L':
						cache[lru].tag=tag;
						cache[lru].last=tik++;
						miss++;
						eviction++;
						break;
					case 'S':
						cache[lru].tag=tag;
						cache[lru].last=tik++;
						miss++;
						eviction++;
						break;
					case 'M':
						cache[lru].tag=tag;
						cache[lru].last=tik++;
						hit++;
						miss++;
						eviction++;
					}
				}
			}else{
				if(cache[i].valid==false){
					if(verbose){
						printf("%s",buf);
						switch(op){
						case 'L':
							printf(" miss\n");
							cache[i].last=tik++;
							cache[i].tag=tag;
							cache[i].valid=true;
							miss++;
							break;
						case 'S':
							printf(" miss\n");
							cache[i].last=tik++;
							cache[i].tag=tag;
							cache[i].valid=true;
							miss++;
							break;
						case 'M':
							printf(" miss hit\n");
							cache[i].last=tik++;
							cache[i].tag=tag;
							cache[i].valid=true;
							miss++;
							hit++;
							break;
						}
					}else{
						switch(op){
						case 'L':
							cache[i].last=tik++;
							cache[i].tag=tag;
							cache[i].valid=true;
							miss++;
							break;
						case 'S':
							cache[i].last=tik++;
							cache[i].tag=tag;
							cache[i].valid=true;
							miss++;
							break;
						case 'M':
							cache[i].last=tik++;
							cache[i].tag=tag;
							cache[i].valid=true;
							miss++;
							hit++;
						}
					}
				}else{
					if(verbose){
						printf("%s",buf);
						switch(op){
						case 'L':
							printf(" hit\n");
							cache[i].last=tik++;
							hit++;
							break;
						case 'S':
							printf(" hit\n");
							cache[i].last=tik++;
							hit++;
							break;
						case 'M':
							printf(" hit hit\n");
							cache[i].last=tik++;
							hit+=2;
						}
					}else{
						switch(op){
						case 'L':
							cache[i].last=tik++;
							hit++;
							break;
						case 'S':
							cache[i].last=tik++;
							hit++;
							break;
						case 'M':
							cache[i].last=tik++;
							hit+=2;
						}
					}
				}
			}
		}
	}
	printSummary(hit, miss, eviction);
	free(cache);	
    return 0;
}
