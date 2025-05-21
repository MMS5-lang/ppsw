#include <LPC21xx.H>
#include "timer.h"

#define TCR_CE (1<<0)
#define TCR_CR (1<<1)
#define MCR_MR0R (1<<1)
#define MCR_MR0I (1<<0)
#define IR_MR0 (1<<0)

#define TICKS_PER_US 15

void InitTimer0(void){
	T0TCR = TCR_CE;
}

void WaitOnTimer0(unsigned int uiTime) {
	T0TCR |= TCR_CR;
	T0TCR &= (~TCR_CR);
	while(T0TC<((uiTime)*TICKS_PER_US)){}
}

void InitTimer0Match0(unsigned int uiDelayTime){
	T0MR0 = uiDelayTime*TICKS_PER_US;
	T0MCR |= (MCR_MR0R|MCR_MR0I);
	T0TCR |= TCR_CR;
	T0TCR &= ~TCR_CR;
	T0TCR |= TCR_CE;
}

void WaitOnTimer0Match0(){
	while((T0IR&IR_MR0)==0){}
	T0IR = IR_MR0;
}
