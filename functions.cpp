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
#include<string.h>
#include<functions.h>
#include<algorithm>
#include<math.h>
using namespace std;
#define clr() printf("\033[H\033[J")
void moveCursor(int x,int y) {
	cout<<"\033["<<x<<";"<<y<<"H";
	fflush(stdout);
}
static struct termios initSettings,newSettings;
static int peek_char = -1;
char const* rootPath;
size_t dirSize=1024;
char currentDir[1024];
#define MAX 10

std::vector<dirent*> files;
int firstIndex=0,lastIndex=firstIndex+MAX,cursor=1;

void setRootPath(char const* path){
	rootPath=path;
}

void printFiles(){
	clr();
	// printf("at: %s\n",currentDir);
	for(int fileIt=firstIndex; fileIt<lastIndex && fileIt<files.size(); fileIt++){
		printf("%d: %s\n",fileIt,files[fileIt]->d_name);
	}
	return;
}
void getSetCurrentDir(char const* dir){
	DIR* dp;
	struct dirent* entry;
	// struct stat statbuf;

	if((dp=opendir(dir))==NULL){
		fprintf(stderr, "Can't open the Directory!\n");
		return;
	}
	chdir(dir);
	getcwd(currentDir,dirSize);
	files.erase(files.begin(),files.end());
	// chdir(dir);
	while((entry=readdir(dp))!=NULL){
		files.push_back(entry);
		// lstat(entry->d_name,&statbuf);
		// TODO if directory then print colored
	}
	closedir(dp);
	firstIndex=0;
	cursor=lastIndex=min(MAX,int(files.size()));
	printFiles();
	moveCursor(cursor,0);
	return;
}
void initKeyboard(){
	tcgetattr(0,&initSettings);
	newSettings=initSettings;
	newSettings.c_lflag &= ~ICANON;
	newSettings.c_lflag &= ~ECHO;
	// newSettings.c_lflag &= ~ISIG;
	newSettings.c_cc[VMIN]=1;
	newSettings.c_cc[VTIME]=0;
	tcsetattr(0,TCSANOW,&newSettings);
	return;
}
void closeKeyboard(){
	tcsetattr(0,TCSANOW,&initSettings);
	return;
}
int kbHit(){
	char ch;
	int nread;
	if(peek_char!=-1) return 1;
	newSettings.c_cc[VMIN]=0;
	tcsetattr(0,TCSANOW,&newSettings);	
	nread=read(0,&ch,1);
	newSettings.c_cc[VMIN]=1;
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
void moveUp(){
	if(cursor>1){
		cursor--;
		moveCursor(cursor,0);
		return;
	}
	if(firstIndex==0) return;
	firstIndex--;
	lastIndex--;
	printFiles();
	moveCursor(cursor,0);
	return;
}
void moveDown(){
	if(cursor<files.size() && cursor<MAX){
		cursor++;
		moveCursor(cursor,0);
		return;
	}
	if(lastIndex==files.size()) return;
	firstIndex++;
	lastIndex++;
	printFiles();
	moveCursor(cursor,0);
	return;
}
void levelUp(){
	if(strcmp(currentDir,rootPath)==0){
		// printf("at root dir!\n");
		return;
	}
	getSetCurrentDir("../");
	return;
}
void goHome(){
	getSetCurrentDir(rootPath);
	return;
}
void openFile(){
	struct stat statbuf;
	char *fileName=files[cursor+firstIndex-1]->d_name;
	lstat(fileName,&statbuf);
	if(S_ISDIR(statbuf.st_mode)){
		getSetCurrentDir((string(currentDir)+'/'+string(fileName)).c_str());
	}
	return;
}
void toggleMode(){
	// printf("\033[XA"); // Move up X lines;
	// printf("\033[XC"); // Move right X column;
	// printf("\033[XD"); // Move left X column
	closeKeyboard();
	printf("\033[10B"); // Move down X lines;
	char input[1024];
	while(1){
		printf(">");
		cin>>input;
		if(strcmp("exit",input)==0){
			initKeyboard();
			printFiles();
			return;
		}
		else{
			cout<<"invalid command";
		}
		// do commands with s
	}
}