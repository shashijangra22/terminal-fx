#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<termios.h>
#include "getchoice.h"

char *menu[]={
	"a - add a new record",
	"d - delete a record",
	"q - quit",
	NULL,
};
int main(){
	int choice=0;
	FILE *input;
	FILE *output;
	struct termios initSettings,newSettings;

	if(!isatty(fileno(stdout))){
		fprintf(stderr, "You are not a terminal\n");
	}
	input=fopen("/dev/tty","r");
	output=fopen("/dev/tty","w");
	if(!input || !output){
		fprintf(stderr, "unable to open /dev/tty\n");
		exit(1);
	}

	tcgetattr(fileno(input),&initSettings);
	newSettings=initSettings;

	newSettings.c_lflag &= ~ICANON;
	newSettings.c_lflag &= ~ECHO;
	newSettings.c_cc[VMIN] = 1;
	newSettings.c_cc[VTIME] = 0;
	newSettings.c_lflag &= ~ISIG;

	if(tcsetattr(fileno(input),TCSANOW,&newSettings)){
		fprintf(stderr, "Can't set attributes!\n");
	}

	do{
		choice=getchoice("Please select an option : ",menu,input,output);
		printf("You have choosen : %c\n",choice);
	}while(choice!='q');
	tcsetattr(fileno(input),TCSANOW,&initSettings);
	exit(0);
}