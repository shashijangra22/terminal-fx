#include<termios.h>
#include<functions.h>
void initKeyboard(){
	tcgetattr(0,&initSettings);
	newSettings=initSettings;
	newSettings.c_lflag &= ~ICANON;
	newSettings.c_lflag &= ~ECHO;
	// newSettings.c_lflag &= ~ISIG;
	newSettings.c_cc[VMIN]=1;
	newSettings.c_cc[VTIME]=0;
	tcsetattr(0,TCSANOW,&newSettings);
	return;
}
void closeKeyboard(){
	tcsetattr(0,TCSANOW,&initSettings);
	return;
}