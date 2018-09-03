// Name: Shashi Jangra
// Email ID: shashi.jangra@students.iiit.ac.in
// Roll Number: 2018202001
// M.Tech CSIS

// this file contains all the definitions of functions declared in functions.h
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
#define clr() printf("\033[H\033[J")	// clears the screen

// global variables to be used throughout the program
static struct termios initSettings,newSettings;
static int peek_char = -1;
char const* rootPath;
size_t dirSize=1024;
char currentDir[1024];
#define MAX 10		// max items to display in normal mode
struct winsize w;
std::vector<string> inputVector;
std::vector<string> backStack;
std::vector<string> forwardStack;
std::vector<dirent*> files;
std::vector<string> searchStack;
int firstIndex=0,lastIndex=firstIndex+MAX,cursor=1;
int modLine;
int outputLine;
int statusLine;
int inputLine;
FILE* out;

// move cursor to x and y coordinate

void moveCursor(int x,int y) {
	cout<<"\033["<<x<<";"<<y<<"H";
	fflush(stdout);
}

// set the root path on program start
void setRootPath(char const* path){
	rootPath=path;
	ioctl(STDOUT_FILENO,TIOCGWINSZ,&w);
	modLine=w.ws_row-5;
	outputLine=modLine+1;
	statusLine=outputLine+1;
	inputLine=statusLine+1;
}

// saves the snapshot in dumpfile

void printDir(char const* dir, int depth){
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
			if(strcmp(".",entry->d_name)==0 || strcmp("..",entry->d_name)==0){
				continue;
			}
			fprintf(out,"%*s%s/\n",depth,"",entry->d_name);
			printDir(entry->d_name,depth+4);
		}
		else{
			fprintf(out,"%*s%s\n",depth,"",entry->d_name);
		}
	}
	chdir("..");
	closedir(dp);
}

// snapshot

void snapshot(string src,string fileName){
	out = fopen((src+'/'+fileName).c_str(),"w");
	printDir(src.c_str(),0);
	fclose(out);
	return;
}

// display search results
void printResults(){
	moveCursor(14,0);
	cout<<"Seach Results are: \n\n";
	for(int x=0; x<searchStack.size(); x++){
		cout<<searchStack[x]<<"\n";
	}
	cout<<"Press any key to go back: ";
	return;
}

// print the files vector

void printFiles(){
	clr();
	struct stat fileInfo;
	// printf("at: %s\n",currentDir);
	for(int fileIt=firstIndex; fileIt<lastIndex && fileIt<files.size(); fileIt++){
		lstat(files[fileIt]->d_name,&fileInfo);
		if((S_ISDIR(fileInfo.st_mode))){
			cout<<fileIt+1<<": "<<"\033[1;31m"<<files[fileIt]->d_name<<"\033[0m";
		}
		else{
			cout<<fileIt+1<<": "<<"\033[1;36m"<<files[fileIt]->d_name<<"\033[0m";
		}
		cout<<"\n";
	}
	moveCursor(modLine,0);
	cout<<"Mode: Normal Mode";
	return;
}

// set the files vector

void getSetCurrentDir(char const* dir){
	DIR* dp;
	struct dirent* entry;

	if((dp=opendir(dir))==NULL){
		fprintf(stderr, "Can't open the Directory!\n");
		return;
	}
	chdir(dir);
	getcwd(currentDir,dirSize);
	files.clear();
	while((entry=readdir(dp))!=NULL){
		files.push_back(entry);
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
	moveCursor(modLine,0);
	for(int i=modLine; i<w.ws_row; i++){
		for(int j=0; j<w.ws_col; j++){
			cout<<" ";
		}
	}
	moveCursor(modLine,0);
	cout<<"Mode: Command Mode";
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

void copyFile(string fileName, string dest){
	char block[1024];
	int in,out;
	int nread;
	in = open(fileName.c_str(),O_RDONLY);
	out = open((dest+'/'+fileName).c_str(),O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR);
	while((nread = read(in,block,sizeof(block)))>0){
		write(out,block,nread);
	}
	return;
}

int deleteFile(string filePath){
	int x = unlink(filePath.c_str());
	return x;
}

void copyDir(string dir, string dest){
	DIR *dp;
	struct dirent *entry;
	struct stat statbuf;

	if((dp = opendir(dir.c_str()))==NULL){
		printf("Can't open the directory: %s",dir.c_str());
		return;
	}
	chdir(dir.c_str()); //  i m in a
	while((entry = readdir(dp))!=NULL){
		lstat(entry->d_name,&statbuf);
		if(S_ISDIR(statbuf.st_mode)){
			if(strcmp(".",entry->d_name)==0 || strcmp("..",entry->d_name)==0){
				continue;
			}
			mkdir((dest+'/'+entry->d_name).c_str(),S_IRUSR|S_IWUSR|S_IXUSR);
			copyDir(entry->d_name,dest+'/'+entry->d_name);
		}
		else{
			copyFile(entry->d_name,dest);
		}
	}
	chdir("..");
	closedir(dp);
	return;
}

void delDir(string dir){
	DIR *dp;
	struct dirent *entry;
	struct stat statbuf;

	if((dp = opendir(dir.c_str()))==NULL){
		fprintf(stderr, "Can't open the directory: %s\n",dir.c_str());
		return;
	}
	chdir(dir.c_str()); //  i m in a
	while((entry = readdir(dp))!=NULL){
		lstat(entry->d_name,&statbuf);
		if(S_ISDIR(statbuf.st_mode)){
			if(strcmp(".",entry->d_name)==0 || strcmp("..",entry->d_name)==0){
				continue;
			}
			delDir(entry->d_name);
			rmdir(entry->d_name);
		}
		else{
			unlink(entry->d_name);
		}
	}
	chdir("..");
	closedir(dp);
}

void searchFiles(string fileName,string dir){
	DIR *dp;
	struct dirent *entry;
	struct stat statbuf;

	if((dp = opendir(dir.c_str()))==NULL){
		fprintf(stderr, "Can't open the directory: %s\n",dir.c_str());
		return;
	}
	chdir(dir.c_str()); //  i m in a
	while((entry = readdir(dp))!=NULL){
		lstat(entry->d_name,&statbuf);
		if(S_ISDIR(statbuf.st_mode)){
			if(strcmp(".",entry->d_name)==0 || strcmp("..",entry->d_name)==0){
				continue;
			}
			searchFiles(fileName,entry->d_name);
		}
		else{
			if(strcmp(fileName.c_str(),entry->d_name)==0){
				char buf[1000];
				getcwd(buf,1000);
				string toPut = string(buf);
				int offset=string(rootPath).size();
				searchStack.push_back(toPut.substr(offset)+'/'+string(entry->d_name));
			}
		}
	}
	chdir("..");
	closedir(dp);
}

void toggleMode(){
	closeKeyboard();
	clrCMD();
	string input;	// user input in command mode
	while(1){
		moveCursor(inputLine,0);
		printf(":");
		getline(cin>>ws,input);
		fillInputVector(input);		// split input to seperate arguments
		if(inputVector[0]=="exit"){	// exit command mode
			getSetCurrentDir(currentDir);
			moveCursor(cursor,0);
			break;
		}
		else if(inputVector[0]=="search"){	// search the file
			string fileName=inputVector[1];
			searchStack.clear();
			searchFiles(fileName,currentDir);
			printResults();
			getchar();
			printFiles();
			moveCursor(cursor,0);
			break;
		}
		else if(inputVector[0]=="move_dir"){		// move directory recursively
			string src,dest;
			int size = inputVector.size();
			if(inputVector[size-1][0]=='~'){
				dest=rootPath+inputVector[size-1].substr(1);
			}
			else if(inputVector[size-1][0]=='/'){
				dest=currentDir+inputVector[size-1];
			}
			else{
				clrCMD();
				moveCursor(statusLine,0);
				cout<<"Invalid Path!";
				continue;
			}
			for(int i=1; i<size-1; i++){
				src=inputVector[i];
				mkdir((dest+'/'+src).c_str(),S_IRUSR|S_IWUSR|S_IXUSR);
				copyDir(currentDir+'/'+src,dest+'/'+src);
				delDir(src);
				rmdir(src.c_str());	
			}
			clrCMD();
			moveCursor(statusLine,0);
			cout<<"Moved Succesfully!";
		}
		else if(inputVector[0]=="delete_dir"){	// delete directory recursively
			string dest;
			if(inputVector[1][0]=='~'){
				dest=rootPath+inputVector[1].substr(1);
			}
			else if(inputVector[1][0]=='/'){
				dest=currentDir+inputVector[1];
			}
			else{
				clrCMD();
				moveCursor(statusLine,0);
				cout<<"Invalid Path!";
				continue;
			}
			delDir(dest);
			rmdir(dest.c_str());
			clrCMD();
			moveCursor(statusLine,0);
			cout<<"Deleted Succesfully!";
		}
		else if(inputVector[0]=="create_file" || inputVector[0]=="create_dir"){	//create file or directory
			string loc;
			int size=inputVector.size();
			if(inputVector[size-1][0]=='~'){
				loc=rootPath+inputVector[size-1].substr(1);
			}
			else if(inputVector[size-1][0]=='/'){
				loc=currentDir+inputVector[size-1];
			}
			else if(inputVector[size-1][0]=='.'){
				loc=currentDir;
			}
			else{
				clrCMD();
				moveCursor(statusLine,0);
				cout<<"Invalid Path!";
				continue;
			}
			for(int i=1; i<size-1; i++){
				string fileName=inputVector[i];
				if(inputVector[0]=="create_file"){
					open((loc+'/'+fileName).c_str(),O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR);
				}
				else{
					mkdir((loc+'/'+fileName).c_str(),S_IRUSR|S_IWUSR|S_IXUSR);
				}
			}
			clrCMD();
			moveCursor(statusLine,0);
			cout<<"Created Succesfully!";
		}
		else if(inputVector[0]=="rename"){	// rename file
			string a = inputVector[1];
			string b = inputVector[2];
			rename(a.c_str(),b.c_str());
			clrCMD();
			moveCursor(statusLine,0);
			cout<<"Renamed Succesfully!";
		}
		else if(inputVector[0]=="move"){	// move a file 
			string fileName,dest;
			int size = inputVector.size();
			if(inputVector[size-1][0]=='~'){
				dest=rootPath+inputVector[size-1].substr(1);
			}
			else if(inputVector[size-1][0]=='/'){
				dest=currentDir+inputVector[size-1];
			}
			else{
				clrCMD();
				moveCursor(statusLine,0);
				cout<<"Invalid Path!";
				continue;
			}
			for(int i=1; i<size-1; i++){
				fileName=inputVector[i];
				copyFile(fileName,dest);
				deleteFile(currentDir+'/'+fileName);
			}
			clrCMD();
			moveCursor(statusLine,0);
			cout<<"Moved Succesfully!";
		}
		else if(inputVector[0]=="goto"){	// go to a specific directory
			if(inputVector[1][0]=='/'){
				goHome();
			}
			else{
				if(inputVector[1][0]=='~'){
					getSetCurrentDir((rootPath+inputVector[1].substr(1)).c_str());
				}
			}
			moveCursor(cursor,0);
			break;
		}
		else if(inputVector[0]=="delete_file"){	// delete a file
			string filePath;
			if(inputVector[1][0]=='~'){
				filePath=rootPath+inputVector[1].substr(1);
			}
			else if(inputVector[1][0]=='/'){
				filePath=currentDir+inputVector[1];
			}
			else{
				clrCMD();
				moveCursor(statusLine,0);
				cout<<"Invalid Path!";
				continue;
			}
			clrCMD();
			moveCursor(statusLine,0);
			if(!deleteFile(rootPath+'/'+filePath)){
				cout<<"Deleted Succesfully!";
			}
			else{
				cout<<"Deletion Failed!";
			}

		}
		else if(inputVector[0]=="snapshot"){	// dump snapshot to dumpfile
			string dirName;
			if(inputVector[1][0]=='~'){
				dirName=rootPath+inputVector[1].substr(1);
			}
			else if(inputVector[1][0]=='/'){
				dirName=currentDir+inputVector[1];
			}
			else{
				clrCMD();
				moveCursor(statusLine,0);
				cout<<"Invalid Path!";
				continue;
			}
			snapshot(dirName,inputVector[2]);
			clrCMD();
			moveCursor(statusLine,0);
			cout<<"Dumpfile Dumped Succesfully!";
		}
		else if(inputVector[0]=="copy_dir"){	// copy directory recursively
			string dirName,dest;
			int size = inputVector.size();
			if(inputVector[size-1][0]=='~'){
				dest=rootPath+inputVector[size-1].substr(1);
			}
			else if(inputVector[size-1][0]=='/'){
				dest=currentDir+inputVector[size-1];
			}
			else{
				clrCMD();
				moveCursor(statusLine,0);
				cout<<"Invalid Path!";
				continue;
			}
			for(int i=1; i<size-1; i++){
				dirName=inputVector[i];
				mkdir((dest+'/'+dirName).c_str(),S_IRUSR|S_IWUSR|S_IXUSR);
				copyDir(currentDir+'/'+dirName,dest+'/'+dirName);
			}
			clrCMD();
			moveCursor(statusLine,0);
			cout<<"Copied Succesfully!";
		}
		else if(inputVector[0]=="copy"){		// copy file
			string fileName,dest;
			int size = inputVector.size();
			if(inputVector[size-1][0]=='~'){
				dest=rootPath+inputVector[size-1].substr(1);
			}
			else if(inputVector[size-1][0]=='/'){
				dest=currentDir+inputVector[size-1];
			}
			else{
				clrCMD();
				moveCursor(statusLine,0);
				cout<<"Invalid Path!";
				continue;
			}
			for(int i=1; i<size-1; i++){
				fileName=inputVector[i];
				copyFile(fileName,dest);
			}
			clrCMD();
			moveCursor(statusLine,0);
			cout<<"Copied Succesfully!";
		}
		else{
			clrCMD();
			moveCursor(statusLine,0);
			cout<<"Status: Invalid Command!";		// show error on invalid command
		}
	}
	initKeyboard();		// set terminal settings to change to normal mode
	return;
}