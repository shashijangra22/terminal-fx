#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<kbhitfunc.h>
int main(){
	char ch='a';
	initKeyboard();
	while(ch!='q'){
		printf("looping...\n");
		sleep(1);
		if(kbhit()){
			ch=readch();
			printf("you hit: %c\n",ch );
		}
	}
	closeKeyboard();
	return 0;
}