#include<stdio.h>
#include<stdlib.h>
#include<termios.h>
#include<unistd.h>
#include<kbhitfunc.h>
static struct termios initSettings,newSettings;
static int peek_char = -1;

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

int kbhit(){
	char ch;
	int nread;
	if(peek_char!=-1){
		return 1;
	}
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

int readch(){
	char ch;
	if(peek_char!=-1){
		ch=peek_char;
		peek_char=-1;
		return ch;
	}
	read(0,&ch,1);
	return ch;
}
