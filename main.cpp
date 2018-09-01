#include<unistd.h>
#include<stdio.h>
#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<functions.h>
#define clr() printf("\033[H\033[J")
using namespace std;
int main(){
	clr();
	char path[1024];
	size_t size=1024;
	getcwd(path,size);
	setRootPath(path);
	getSetCurrentDir(path);
	initKeyboard();
	char ch='a';
	while(ch!='q'){
		if(kbHit()){
			ch=readCh();
			switch(ch){
				case 65:moveUp();
					break;
				case 66:moveDown();
					break;
				case 127:levelUp();
					break;
				case 'h':goHome();
					break;
				case ':':toggleMode();
					break;
				default:
					break;
			}
		}
	}
	closeKeyboard();
	exit(0);
}