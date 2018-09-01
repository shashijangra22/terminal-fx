#include<unistd.h>
#include<printDirFunc.h>
#include<stdio.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<string.h>
#include<dirent.h>
void listFiles(char *dir){
	DIR *dp;
	struct dirent *entry;
	struct stat statbuf;

	if((dp=opendir(dir))==NULL){
		fprintf(stderr, "Can't open the Directory!\n");
	}
	chdir(dir);
	while((entry=readdir(dp))!=NULL){
		// save in a data structure
		lstat(entry->d_name,&statbuf);
		// TODO if directory then print colored
		printf("%s\n",entry->d_name);
	}
	chdir("..");
	closedir(dp);
	return;
}