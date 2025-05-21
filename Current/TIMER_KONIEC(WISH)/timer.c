#include <LPC21xx.H>
#include "timer.h"

#define COUNTER0_ENABLE_bm (1<<0)
#define COUNTER0_RESET_bm (1<<1)
#define RESET_ON_MR0_bm (1<<1)
#define INTERRUPT_ON_MR0_bm (1<<0)
#define MR0_INTERRUPT_FLAG_bm (1<<0)

#define TICKS_PER_US 15

void InitTimer0(void){
	T0TCR = COUNTER0_ENABLE_bm; //wlaczam timer
}

void WaitOnTimer0(unsigned int uiTime) {
	
	T0TCR = (T0TCR | COUNTER0_RESET_bm); //resetuje timer
	T0TCR = (T0TCR & (~COUNTER0_RESET_bm)); //zerowanie flagi resetu
	
	while(T0TC<((uiTime)*TICKS_PER_US)){}
}

void InitTimer0Match0(unsigned int uiTime){
	
	T0MR0 = uiTime*TICKS_PER_US;  //ustawiane ilosci impulsow
	
	T0MCR |= (RESET_ON_MR0_bm|INTERRUPT_ON_MR0_bm); // ustawiam timer aby ustawil flage i resetuje timer
	
	T0TCR |= COUNTER0_RESET_bm; //resetuje timer
	T0TCR &= ~COUNTER0_RESET_bm; //zerowanie flagi resetu
	T0TCR |= COUNTER0_ENABLE_bm; //wlaczam timer
}

void WaitOnTimer0Match0(){
	while((T0IR&MR0_INTERRUPT_FLAG_bm)==0){}
	T0IR = MR0_INTERRUPT_FLAG_bm; //kasuje flage przerwania
}
