#include<stdbool.h>
#include<stdio.h>

int main(int argc,char **argv){
	char buf[50];
	printf("%s",argv[1]);
	FILE* file=fopen(argv[1],"r");
	int lines=0;
	char op;
	long long add;
	while(fgets(buf,50,file)){
		if(buf[0]=='I'){
			continue;
		}else{
		sscanf(buf," %c %llx",&op,&add);
		printf("%c %llx\n",op,add);
		lines++;
		}
	}
	printf("%d",lines);
}
