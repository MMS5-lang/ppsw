#include "timer_interrupts.h"
#include "led.h"
#include "keyboard.h"


void Automat() {
	enum LedState {LED_STOP, LED_LEFT, LED_RIGHT};
	static enum LedState eLedState = LED_STOP;
	
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
		}
}

int main (){
	unsigned int iMainLoopCtr;
	LedInit();
	Timer0Interrupts_Init(20000, &Automat);

	while(1){
		iMainLoopCtr++;
		iMainLoopCtr++;
		iMainLoopCtr++;
		iMainLoopCtr++;
		iMainLoopCtr++;
	}
}
