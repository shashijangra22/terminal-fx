#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<printDirFunc.h>
int main(int argc, char *argv[])
{
	if(argc<2){
		printf("Usage: ./printDir <directory_path>\n");
		exit(0);
	}
	printf("Directory scan for %s: \n",argv[1]);
	printDir(argv[1],0);
	exit(0);
}