#include <LPC21xx.H>
#include "timer.h"

#define COUNTER0_ENABLE_bm  (1 << 0) //str 217 w manualu najmlodszy bit rejestru T0TCR wlacza zliczanie cykli
#define COUNTER0_RESET_bm   (1 << 1) //drugi od najmlodszego bitu resetuje rejestr ze zliczonymi cyklami

#define T0MCR_INTERRUPT_ON_MR0_bm (1 << 0)  // Bit 0 w T0MCR: Przerwanie przy dopasowaniu MR0
#define T0MCR_RESET_ON_MR0_bm    (1 << 1)  // Bit 1 w T0MCR: Reset przy dopasowaniu MR0
#define T0IR_MR0_INTERRUPT_bm    (1 << 0)  // Bit 0 w T0IR: Flaga przerwania dla MR0

#define PCLK_FREQ 15 //zegar taktowania wynosi 1/4 taktowania rdzenia procesora, czyli 15 000 cykli na milisekunde, 15 cykli na mikrosekunde

void InitTimer0(void){
	T0TCR = COUNTER0_ENABLE_bm; //uruchamia nam zliczanie cykli zegara
}

void WaitOnTimer0(unsigned int uiTime) {
	
	T0TCR = COUNTER0_RESET_bm; //reset timera0
	T0TCR = ~(COUNTER0_RESET_bm); //usuniecie flagi resetu
	
	while(T0TC !=((uiTime)*PCLK_FREQ)){} //aby to dzialalo powinnismi zmiejszyc czestotliosc zliczania pclk
}

//informacje o rejestrze, ktory ustawia 'porownania' z rejestrem zliczajacym sa w manualu na stronie 218 (T0MCR T1MCR), tzw blok porownujacy
/*
po kolei bity rejestru T0MRC (stan wysoki):
0 - przerwanie po osiagnieciu wartosci wpisanej do rejestru MR0
1 - RESET po osiagnieciu wartosci wpisanej do rejestru z rejestru MR0
2 - STOP po osiagnieciu wartosci wpisanej do rejestru z rejestru MR0
3 - analogicznie nastepny rejestr MR1
...
*/

void InitTimer0Match0(unsigned int iDelayTime) {
    T0MR0 = (iDelayTime * PCLK_FREQ);  // PCLK = 15 MHz, 1 �s = 15 cykli, to co wyzej
    T0MCR = (T0MCR | (T0MCR_INTERRUPT_ON_MR0_bm | T0MCR_RESET_ON_MR0_bm)); //po osiagnieciu wartosci MR0 przez timer0 wysyla flage przerwania i zeruje sie timer
	
    T0TCR = COUNTER0_RESET_bm; //resetuje wartosc timera
		T0TCR = ~(COUNTER0_RESET_bm); //usuwa flage resetu timera
}

void WaitOnTimer0Match0(void) {
    while ((T0IR & T0IR_MR0_INTERRUPT_bm) == 0) {} // Czekanie na flage przerwania w T0IR
    T0IR = T0IR | T0IR_MR0_INTERRUPT_bm; // Czyszczenie flagi przerwania (zapis 1 do bitu 0 w T0IR), manual str 217
}
//  T0IR                              1111
//  T0IR_MR0_INTERRUPT_bm             0001
//                                  | 1111
