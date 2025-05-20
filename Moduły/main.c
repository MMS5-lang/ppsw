#include <LPC21xx.H>
#include "led.h"
#include "keyboard.h"



void Delay(unsigned int uiMilliSeconds){
	unsigned int uiSecondsLimit;
	unsigned int uiHelpCounter = uiMilliSeconds *12000;
	for(uiSecondsLimit = 0 ; uiSecondsLimit<uiHelpCounter ; uiSecondsLimit++){}
}


int main(){

}
