#include "led.h"
#include "keyboard.h"



void Delay(unsigned int uiMilliSeconds){
	unsigned int uiSecondsLimit;
	unsigned int uiHelpCounter = uiMilliSeconds *12000;
	for(uiSecondsLimit = 0 ; uiSecondsLimit<uiHelpCounter ; uiSecondsLimit++){}
}




/* Zad 1
enum eLedState {LED_LEFT, LED_RIGHT};
enum eLedState eLedState = LED_LEFT;
int main() {
    LedInit();
    KeyboardInit(); 
    while (1) {
        switch (eLedState) {
            case LED_LEFT:
                LedStepLeft();
                eLedState = LED_RIGHT;
                break;
            case LED_RIGHT:
                LedStepRight();
                eLedState = LED_LEFT;
                break;
        }
        Delay(250); 
    }
}
*/

/* ZAD 2
enum eLedState {STATE0, STATE1, STATE2, STATE3, STATE4, STATE5};
enum eLedState eLedState = STATE0;

int main() {
    LedInit();
    KeyboardInit();
    while (1) {
        switch (eLedState) {
            case STATE0:
                LedStepLeft();
                eLedState = STATE1;
                break;
            case STATE1:
                LedStepLeft();
                eLedState = STATE2;
                break;
            case STATE2:
                LedStepLeft();
                eLedState = STATE3;
                break;
            case STATE3:
                LedStepRight();
                eLedState = STATE4;
                break;
            case STATE4:
                LedStepRight();
                eLedState = STATE5;
                break;
            case STATE5:
                LedStepRight();
                eLedState = STATE0;
                break;
        }
        Delay(250);
    }
}
*/

/* ZAD 3
enum eLedState {MOVE_LEFT, MOVE_RIGHT};
enum eLedState eLedState = MOVE_LEFT;
unsigned char ucStepCounter = 0;

int main() {
    LedInit();
    KeyboardInit();
    while (1) {
        switch (eLedState) {
            case MOVE_LEFT:
                LedStepLeft();
                ucStepCounter++;
                if (ucStepCounter == 3) {
                    eLedState = MOVE_RIGHT;
                    ucStepCounter = 0;
                }
                break;
            case MOVE_RIGHT:
                LedStepRight();
                ucStepCounter++;
                if (ucStepCounter == 3) {
                    eLedState = MOVE_LEFT;
                    ucStepCounter = 0;
                }
                break;
        }
        Delay(250);
    }
}

*/ 

/* Zad 4
enum eLedState {STOPPED, MOVE_RIGHT};
enum eLedState eLedState = MOVE_RIGHT;
unsigned char ucStepCounter = 0;

int main() {
    LedInit();
    KeyboardInit();
    while (1) {
        switch (eLedState) {
            case STOPPED:
                if (eKeyboardRead() == BUTTON_0) {
                    eLedState = MOVE_RIGHT;
                    ucStepCounter = 0;
                }
                break;
            case MOVE_RIGHT:
                if (ucStepCounter < 3) {
                    LedStepRight();
                    ucStepCounter++;
                } else {
                    eLedState = STOPPED;
                }
                break;
        }
        Delay(250);
    }
}
*/

/* Zad 5
enum eLedState {STOPPED, MOVE_RIGHT};
enum eLedState eLedState = MOE_RIGHT;
unsigned char ucStepCounter = 0;

int main() {
    LedInit();
    KeyboardInit();
    while (1) {
        switch (eLedState) {
            case STOPPED:
                if (eKeyboardRead() == BUTTON_0) {
                    eLedState = STOPPED;
                } else if (eKeyboardRead() == BUTTON_1){
									eLedState = MOVE_RIGHT;
								}
                break;
            case MOVE_RIGHT:
                if (eKeyboardRead() != BUTTON_0) {
                    LedStepRight();
                } else {
                    eLedState = STOPPED;
                }
                break;
        }
        Delay(250);
    }
}
*/

enum eLedState {STOPPED, MOVE_LEFT, MOVE_RIGHT, WIPPER, WAIT};
enum eLedState eLedState = STOPPED;

int main() {
    LedInit();
    KeyboardInit();
    while (1) {
			
			static unsigned char sucStep = 0;
			
        switch (eLedState) {
					
            case STOPPED:
                if (eKeyboardRead() == BUTTON_0) {
                    eLedState = MOVE_LEFT;
                } else if (eKeyboardRead() == BUTTON_2) {
                    eLedState = MOVE_RIGHT;
                }
                break;
								
            case MOVE_LEFT:
                if (eKeyboardRead() == BUTTON_1) {
                    eLedState = STOPPED;
                } else if (eKeyboardRead() == BUTTON_3){
										sucStep = 0;
										eLedState = WIPPER;
								} else {
                    LedStepLeft();
                }
                break;
								
            case MOVE_RIGHT:
                if (eKeyboardRead() == BUTTON_1) {
                    eLedState = STOPPED;
                } else {
                    LedStepRight();
                }
                break;
								
						case WIPPER:
							if (sucStep >= 4) {
								eLedState = WAIT;
							} else {
                  if ((sucStep == 0) || (sucStep == 2)){
                   LedStepLeft();
									 sucStep++;
                    } else {
                     LedStepRight();
										 sucStep++;
                    }
                }
                break;
            case WAIT:
                if (eKeyboardRead() == BUTTON_2) {
                    eLedState = MOVE_RIGHT;
                } else {
                    eLedState = WAIT;
                }
                break;
        }
        Delay(100);
    }
}

