// Name: Shashi Jangra
// Email ID: shashi.jangra@students.iiit.ac.in
// Roll Number: 2018202001
// M.Tech CSIS


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
	setRootPath(path);	// set the root path for program
	getSetCurrentDir(path);
	initKeyboard();	// initialise terminal settings
	char ch='a';
	while(ch!='q'){
		if(kbHit()){
			ch=readCh();		// take a character as user input and respond instantly
			switch(ch){
				case 65:scrollUp();		// scroll up on pressing up arrow
					break;
				case 66:scrollDown();	// scroll down on down arrow
					break;
				case 127:goUp();		// go up one level on backspace
					break;
				case 68:goBack();		// go back on left arrow
					break;
				case 67:goForward();	// go forward on right arrow
					break;
				case 'h':goHome();		// go to root path
					break;
				case 10:openFile();		// opens a directory and file on pressing enter
					break;
				case ':':toggleMode();	// switch to command mode with ':' 
					break;
				default:
					break;
			}
		}
	}
	closeKeyboard();	// restore terminal settings
	exit(0);
}