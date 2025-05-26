#include <LPC21xx.H>
#include "led.h"
#define LED0_bm (1 << 16)
#define LED1_bm (1 << 17)
#define LED2_bm (1 << 18)
#define LED3_bm (1 << 19)

enum eDirection {LEFT, RIGHT};

void LedInit(){
	IO1DIR = IO1DIR | (LED0_bm | LED1_bm | LED2_bm | LED3_bm);
	IO1SET=LED0_bm;
}

void LedOn(unsigned char ucLedIndex) {
	
	
	switch(ucLedIndex){
		case 0:
			IO1SET = LED0_bm;
			break;
		case 1:
			IO1SET = LED1_bm;
			break;
		case 2:
			IO1SET = LED2_bm;
			break;
		case 3:
			IO1SET = LED3_bm;
		default:
			IO1CLR = (LED0_bm | LED1_bm | LED2_bm | LED3_bm);
			break;
	}
}
 

void LedStep(enum eDirection eDirection) {
		static unsigned int uiCurrentLedIndex = 0;
    if (eDirection == LEFT) {
        uiCurrentLedIndex = (uiCurrentLedIndex + 1) % 4;
    } else {
        uiCurrentLedIndex = (uiCurrentLedIndex - 1) % 4;
    }
    LedOn(uiCurrentLedIndex);
}

void LedStepLeft(void) {
    LedStep(LEFT);
}


void LedStepRight(void) {
    LedStep(RIGHT);
}

