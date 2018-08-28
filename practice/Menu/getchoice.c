#include<stdio.h>
#include<stdlib.h>
char getchoice(char *greet, char *choices[],FILE *in, FILE *out){
	int chosen=0;
	int selected;
	char **option;
	do{
		fprintf(out,"Choice : %s\n",greet);
		option=choices;
		while(*option){
			fprintf(out,"%s\n",*option);
			option++;
		}
		do{
			selected=fgetc(in);
		}while(selected=='\n' || selected=='\r');
		option=choices;
		while(*option){
			if(selected==*option[0]){
				chosen=1;
				break;
			}
			option++;
		}
		if(!chosen){
			fprintf(out,"Incorrect choice select again!\n");
		}
	}while(!chosen);
	return selected;
}
