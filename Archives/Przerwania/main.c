#include "led.h"
#include "timer_interrupts.h"
#include "keyboard.h"


void Automat(){
		
		enum eLedState {STATE_STOP, STATE_RIGHT, STATE_LEFT};
		static enum eLedState seLedState = STATE_STOP;

		switch(seLedState){
					
				case STATE_RIGHT:
						if(eKeyboardRead() == BUTTON_1){
										seLedState = STATE_STOP;
								}else{
										LedStepRight();
										seLedState = STATE_RIGHT;
								}
								break;
								
				case STATE_LEFT:
						if(eKeyboardRead() == BUTTON_1){
										seLedState = STATE_STOP;
								}
								else{
										LedStepLeft();
										seLedState = STATE_LEFT;
								}
								break;
						
				case STATE_STOP:
						if(eKeyboardRead() == BUTTON_0){
										seLedState = STATE_LEFT;
								}
								else if(eKeyboardRead() == BUTTON_2){
										seLedState = STATE_RIGHT;
								}
								else{
										seLedState = STATE_STOP;
								}
								break;
								
				default:
								break;
		}
}


int main (){
	
		unsigned int iMainLoopCtr;
	
		Timer0Interrupts_Init(20000, &Automat);
		LedInit();
		KeyboardInit();

		while(1){
			
				iMainLoopCtr++;

		}
}
