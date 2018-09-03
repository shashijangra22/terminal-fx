#include<stdio.h>
#include<iostream>
#include<termios.h>
#include<stdlib.h>
#include<string.h>
using namespace std;
int main(){
	struct termios initSettings, newSettings;
	char password[8];
	
	tcgetattr(fileno(stdin),&initSettings);
	
	newSettings=initSettings;
	newSettings.c_lflag&=~ECHO;

	if(tcsetattr(fileno(stdin),TCSAFLUSH,&newSettings)){
		fprintf(stderr, "Could not change settings!\n");
	}
	else{
		printf("Enter password: \n");
		fgets(password,8,stdin);
		tcsetattr(fileno(stdin),TCSAFLUSH,&initSettings);
		fprintf(stdout,"Your entered: %s\n",password);
	}
	return 0;
}
