#include<unistd.h>
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<termios.h>
#include<pwd.h>
#include<grp.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/ioctl.h>
#include<string.h>
#include<vector>
#include<dirent.h>
#include<functions.h>
#include<algorithm>
#include<math.h>
using namespace std;
#define clr() printf("\033[H\033[J")
static struct termios initSettings,newSettings;
static int peek_char = -1;
char const* rootPath;
size_t dirSize=1024;
char currentDir[1024];
#define MAX 20
struct winsize w;
std::vector<string> inputVector;
std::vector<string> backStack;
std::vector<string> forwardStack;
std::vector<dirent*> files;
int firstIndex=0,lastIndex=firstIndex+MAX,cursor=1;
bool ModeBit=false;
void moveCursor(int x,int y) {
	cout<<"\033["<<x<<";"<<y<<"H";
	fflush(stdout);
}
void setRootPath(char const* path){
	rootPath=path;
	ioctl(STDOUT_FILENO,TIOCGWINSZ,&w);
}

void printFiles(){
	clr();
	int i=1;
	struct stat fileInfo;
	// printf("at: %s\n",currentDir);
	for(int fileIt=firstIndex; fileIt<lastIndex && fileIt<files.size(); fileIt++,i++){
		lstat(files[fileIt]->d_name,&fileInfo);
		if((S_ISDIR(fileInfo.st_mode))){
			cout<<fileIt<<": "<<"\033[1;31m"<<files[fileIt]->d_name<<"\033[0m";
		}
		else{
			cout<<fileIt<<": "<<"\033[1;36m"<<files[fileIt]->d_name<<"\033[0m";
		}
		moveCursor(i,30);
		cout<<fileInfo.st_size<<"B";
		moveCursor(i,40);
		cout<<(S_ISDIR(fileInfo.st_mode) ? "d" : "-");
	    cout<<((fileInfo.st_mode & S_IRUSR) ? "r" : "-");
	    cout<<((fileInfo.st_mode & S_IWUSR) ? "w" : "-");
	    cout<<((fileInfo.st_mode & S_IXUSR) ? "x" : "-");
	    cout<<((fileInfo.st_mode & S_IRGRP) ? "r" : "-");
	    cout<<((fileInfo.st_mode & S_IWGRP) ? "w" : "-");
	    cout<<((fileInfo.st_mode & S_IXGRP) ? "x" : "-");
	    cout<<((fileInfo.st_mode & S_IROTH) ? "r" : "-");
	    cout<<((fileInfo.st_mode & S_IWOTH) ? "w" : "-");
	    cout<<((fileInfo.st_mode & S_IXOTH) ? "x" : "-");
	    struct passwd *pw = getpwuid(fileInfo.st_uid);
		struct group  *gr = getgrgid(fileInfo.st_gid);
		cout<<" "<<gr->gr_name<<" "<<pw->pw_name<<" ";
		string time = ctime(&fileInfo.st_mtime);
		cout<<time.substr(4,12)<<"\n";
	}
	moveCursor(w.ws_row-5,0);
	// string appMode=(ModeBit)?"Command Mode":"Normal Mode";
	printf("Mode: Normal Mode");
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
	files.clear();
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
void scrollUp(){
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
void scrollDown(){
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
void goUp(){
	if(strcmp(currentDir,rootPath)==0){
		// printf("at root dir!\n");
		return;
	}
	backStack.push_back(string(currentDir));
	getSetCurrentDir("../");
	return;
}
void goHome(){
	if(strcmp(currentDir,rootPath)==0) return;
	backStack.push_back(string(currentDir));
	getSetCurrentDir(rootPath);
	return;
}
void goBack(){
	if(!backStack.size()){
		return;
	}
	string toVisit = backStack.back();
	backStack.pop_back();
	forwardStack.push_back(string(currentDir));
	getSetCurrentDir(toVisit.c_str());
	return;
}
void goForward(){
	if(!forwardStack.size()){
		return;
	}
	string toVisit = forwardStack.back();
	forwardStack.pop_back();
	backStack.push_back(string(currentDir));
	getSetCurrentDir(toVisit.c_str());
	return;
}
void openFile(){
	struct stat statbuf;
	char *fileName=files[cursor+firstIndex-1]->d_name;
	lstat(fileName,&statbuf);
	if(S_ISDIR(statbuf.st_mode)){
		if(strcmp(fileName,"..")==0 && strcmp(currentDir,rootPath)==0) return;
		if(strcmp(fileName,".")==0) return;
		backStack.push_back(string(currentDir));
		getSetCurrentDir((string(currentDir)+'/'+string(fileName)).c_str());
	}
	else{
		pid_t pid=fork();
		if(pid==0){
			execl("/usr/bin/xdg-open","xdg-open",fileName,NULL);
			exit(1);
		}
	}
	return;
}
void clrCMD(){
	for(int i=0; i<4; i++){
		for(int j=0; j<w.ws_col; j++){
			cout<<" ";
		}
	}
}

void fillInputVector(string input){
	inputVector.clear();
	for(int i=0; i<input.size(); i++){
		string temp;
		int x=i;
		while(input[i]!=' ' && i<input.size()){
			if(input[i]==92){
				temp+=" ";
				i+=2;
			}
			else{
				temp+=input[i];
				i++;
			}
		}
		inputVector.push_back(temp);
	}
	return;
}

void copyFile(string src, string dest){
	char block[1024];
	int in,out;
	int nread;
	in = open(src.c_str(),O_RDONLY);
	out = open((dest+'/'+src).c_str(),O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR);
	while((nread = read(in,block,sizeof(block)))>0){
		write(out,block,nread);
	}
	return;
}

void deleteFile(string filePath){
	int x = unlink(filePath.c_str());
	if(x){
		cout<<"bakchodi!";
	}
	return;
}
void toggleMode(){
	int modLine=w.ws_row-5;
	int statusLine=modLine+1;
	int inputLine=statusLine+1;
	closeKeyboard();
	moveCursor(modLine,0);
	printf("Mode: Command Mode\n");
	string input;
	while(1){
		moveCursor(inputLine,0);
		printf(":");
		getline(cin>>ws,input);
		fillInputVector(input);
		if(inputVector[0]=="exit"){
			// printFiles();
			moveCursor(cursor,0);
			break;
		}
		else if(inputVector[0]=="delete_file"){
			// int size=inputVector.size();
			string filePath=inputVector[1];
			deleteFile(rootPath+filePath);
			moveCursor(statusLine,0);
			cout<<"Deleted!";
			moveCursor(inputLine,0);
		}
		else if(inputVector[0]=="copy"){
			string src,dest;
			int size = inputVector.size();
			for(int i=1; i<size-1; i++){
				if(inputVector[size-1][0]=='~'){
					dest=rootPath+inputVector[size-1].substr(1);
				}
				else{
					dest=currentDir+'/'+inputVector[size-1];
				}
				src=inputVector[i];
				copyFile(src,dest);
				moveCursor(statusLine,0);
				cout<<"Copied!";
				moveCursor(inputLine,0);
			}
		}
		else{
			moveCursor(statusLine,0);
			cout<<"Status: Invalid Command!\t\t";
			moveCursor(inputLine,0);
			clrCMD();
		}
	}
	initKeyboard();
	return;
}