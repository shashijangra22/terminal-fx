#include<unistd.h>
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<termios.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<string.h>
#include<vector>
#include<dirent.h>
#include<functions.h>
static struct termios initSettings,newSettings;
static int peek_char = -1;
#define clr() printf("\033[H\033[J")
std::vector<dirent*> files;
int i=0,j=i+10;
void printFiles(){
	clr();
	for(int x=i; x<j && x<files.size(); x++){
		printf("%d: %s\n",x,files[x]->d_name);
	}
}
void listFiles(char const* dir){
	DIR* dp;
	struct dirent* entry;
	struct stat statbuf;

	if((dp=opendir(dir))==NULL){
		fprintf(stderr, "Can't open the Directory!\n");
		return;
	}
	chdir(dir);
	while((entry=readdir(dp))!=NULL){
		files.push_back(entry);
		// lstat(entry->d_name,&statbuf);
		// TODO if directory then print colored
	}
	printFiles();
	chdir("..");
	closedir(dp);
	return;
}
void initKeyboard(){
	tcgetattr(0,&initSettings);
	newSettings=initSettings;
	newSettings.c_lflag &= ~ICANON;
	newSettings.c_lflag &= ~ECHO;
	newSettings.c_lflag &= ~ISIG;
	newSettings.c_cc[VMIN]=1;
	newSettings.c_cc[VTIME]=0;
	tcsetattr(0,TCSANOW,&newSettings);
}
void closeKeyboard(){
	tcsetattr(0,TCSANOW,&initSettings);
}
int kbHit(){
	char ch;
	int nread;
	if(peek_char!=-1) return 1;
	newSettings.c_cc[VMIN]=0;
	tcsetattr(0,TCSANOW,&newSettings);	
	nread=read(0,&ch,1);
	newSettings.c_cc[VMIN]=0;
	tcsetattr(0,TCSANOW,&newSettings);
	if(nread==1){
		peek_char=ch;
		return 1;
	}
	return 0;
}

int readCh(){
	char ch;
	if(peek_char!=-1){
		ch=peek_char;
		peek_char=-1;
		return ch;
	}
	read(0,&ch,1);
	return ch;
}
void moveUp(int c){
	if(i==0) return;
	i--;
	j--;
	printFiles();
}
void moveDown(int c){
	if(j==files.size()) return;
	i++;
	j++;
	printFiles();
}