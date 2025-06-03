#include "led.h"
#include "keyboard.h"

enum LedState {LED_STOP, LED_LEFT, LED_RIGHT, BLINK};

void delay(int iDelayTime) {
	unsigned int iLoopCnt;
  for (iLoopCnt = 0; iLoopCnt < iDelayTime * 1130; iLoopCnt++);
}
unsigned char ucBlinkCounter = 0;

int main(void) {
	enum LedState eLedState = LED_STOP;

  LedInit();
  KeyboardInit();

  while(1){
		switch(eLedState){
			case LED_STOP:
				if (eKeyboardRead() == BUTTON_0) {
					eLedState = LED_LEFT;
				}
				else if (eKeyboardRead() == BUTTON_2) {
					eLedState = LED_RIGHT;
				}
				else {
					break;
				}
				break;

			case LED_LEFT:
        if (eKeyboardRead() == BUTTON_1) {
					eLedState = LED_STOP;
        }
				else if (eKeyboardRead() == BUTTON_3){
					eLedState = BLINK;
				}
				else {
					LedStepLeft();
				}
        break;

      case LED_RIGHT:
        if (eKeyboardRead() == BUTTON_1) {
					eLedState = LED_STOP;
        }
				else {
					LedStepRight();
				}
        break;
				
			case BLINK:
				if (ucBlinkCounter == 18) {
					ucBlinkCounter = 0;
					eLedState = LED_RIGHT;
				} else {
					ucBlinkCounter++;
					if ((ucBlinkCounter % 2) == 0) {
							LedOn(3);
					} else {
							LedOn(4);
					}
					eLedState = BLINK;
				}
				break;
    }
    delay(100);
	}
}
