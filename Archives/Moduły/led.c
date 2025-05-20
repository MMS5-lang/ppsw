#include <LPC21xx.H>
#include "led.h"
#define LED0_bm (1 << 16)
#define LED1_bm (1 << 17)
#define LED2_bm (1 << 18)
#define LED3_bm (1 << 19)
#define ALL_LEDS_bm (LED0_bm | LED1_bm | LED2_bm | LED3_bm)

void LedInit(){
	IO1DIR = IO1DIR | ALL_LEDS_bm;
	IO1SET=LED0_bm;
}

void LedOn(unsigned char ucLedIndex) {
    if (ucLedIndex > 3) {
        return; 
    }
    IO1CLR = ALL_LEDS_bm;
    IO1SET = (LED0_bm << ucLedIndex);
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

