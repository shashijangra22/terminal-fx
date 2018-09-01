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
	char const* path="./";
	listFiles(path);
	initKeyboard();
	char ch='a';
	while(ch!='q'){
		// printf("looping...\n");
		// sleep(1);
		if(kbHit()){
			ch=readCh();
			// printf("you hit: %c\n",ch);
			switch(ch){
				case 65:moveUp(1);
					break;
				case 66:moveDown(1);
					break;
				default:
					break;
			}
		}
	}
	closeKeyboard();
	exit(0);
}