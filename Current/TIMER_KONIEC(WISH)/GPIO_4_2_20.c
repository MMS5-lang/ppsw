#include "led.h"
#include "timer.h"

/*
void delay(int iDelayTime) {
	unsigned int iLoopCnt;
  for (iLoopCnt = 0; iLoopCnt < iDelayTime * 1130; iLoopCnt++);
}
*/

int main() {
	LedInit();
	InitTimer0Match0(100000);
	while(1){
		WaitOnTimer0Match0();
		LedStepRight();
	}
}
