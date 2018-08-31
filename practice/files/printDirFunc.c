#include<unistd.h>
#include<printDirFunc.h>
#include<stdio.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<string.h>
#include<dirent.h>
void printDir(char *dir,int depth){
	DIR *dp;
	struct dirent *entry;
	struct stat statbuf;

	if((dp = opendir(dir))==NULL){
		fprintf(stderr, "Can't open the directory: %s\n",dir);
		return;
	}
	chdir(dir);
	while((entry = readdir(dp))!=NULL){
		lstat(entry->d_name,&statbuf);
		if(S_ISDIR(statbuf.st_mode)){
			// found a directory
			if(strcmp(".",entry->d_name)==0 || strcmp("..",entry->d_name)==0){
				continue;
			}
			printf("%*s%s/\n",depth,"",entry->d_name);
			printDir(entry->d_name,depth+4);
		}
		else{
			printf("%*s%s\n",depth,"",entry->d_name);
		}
	}
	chdir("..");
	closedir(dp);
}