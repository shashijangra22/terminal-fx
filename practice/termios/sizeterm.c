#include<stdio.h>
#include<stdlib.h>
#include<term.h>
int main(){
	int rows,cols;
	setupterm(NULL,fileno(stdin),(int*)0);
	rows=tigetnum("lines");
	cols=tigetnum("cols");
	printf("this terminal has %d rows and %d cols/\n",rows,cols );
	return 0;
}