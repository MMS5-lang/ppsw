/*
Wejscie (IntEnable): Na poczatku VIC ma uklad, kt�ry pozwala wlaczac lub wylaczac przerwania z poszczeg�lnych kanal�w (1-wlaczone 0-wylaczone)
Sloty (VectorInterrupt0-15): VIC ma 16 "slot�w" � to takie miejsca, gdzie mozna przypisac konkretne przerwania i powiedziec, co z nimi robic
	Kazdy slot ma:
	Bit Enable (w rejestrze VectorCtl): Wlacza lub wylacza slot - 5 bit
	Bity Source (tez w VectorCtl): Okreslaja, kt�ry kanal (od 0 do 31) ten slot obsluguje.
	Adres procedury (VectorAddr): Miejsce, gdzie zapisany jest adres programu (ISR), kt�ry ma sie uruchomic, gdy przerwanie z tego kanalu sie pojawi, inaczej priorytet.
	
	Przyklad:
		1.Jakies zr�dlo (np. TIMER0 na kanale 4) wywoluje przerwanie.
		2.VIC sprawdza, czy kanal 4 jest wlaczony w IntEnable.
		3.Szuka wsr�d 16 slot�w, czy kt�rys jest przypisany do kanalu 4 i wlaczony.
		4.Jesli znajdzie (np. slot 5), bierze adres ISR z rejestru VectorAddr5.
		5.Mikrokontroler przerywa swoje zadanie i uruchamia ten ISR.

*/

/* 1 
#include <LPC21xx.H>
#include "led.h"

// TIMER
#define mCOUNTER_ENABLE (1 << 0)
#define mCOUNTER_RESET  (1 << 1)

// CompareMatch
#define mINTERRUPT_ON_MR0 (1 << 0)
#define mRESET_ON_MR0     (1 << 1)
#define mMR0_INTERRUPT    (1 << 0)

// VIC (Vector Interrupt Controller) VICIntEnable
#define VIC_TIMER1_CHANNEL_NR 5

// VICVectCntlx Vector Control Registers
#define mIRQ_SLOT_ENABLE 0x00000020


//(Interrupt Service Routine) of Timer 0 interrupt
__irq void Timer1IRQHandler(){

	T1IR=mMR0_INTERRUPT; 	// skasowanie flagi przerwania 
	LedStepRight();		// cos do roboty, tutaj breakpoint
	VICVectAddr=0x00; 	// potwierdzenie wykonania procedury obslugi przerwania
}

void Timer1Interrupts_Init(unsigned int uiPeriod){ // microseconds

        // interrupts

	VICIntEnable |= (0x1 << VIC_TIMER1_CHANNEL_NR);            // Enable Timer 0 interrupt channel 
	VICVectCntl1  = mIRQ_SLOT_ENABLE | VIC_TIMER1_CHANNEL_NR;  // Enable Slot 0 and assign it to Timer 0 interrupt channel
	VICVectAddr1  =(unsigned long)Timer1IRQHandler; 	   // Set to Slot 0 Address of Interrupt Service Routine 

        // match module

	T1MR0 = 15 * uiPeriod;                 	      // value 
	T1MCR |= (mINTERRUPT_ON_MR0 | mRESET_ON_MR0); // action 

        // timer

	T1TCR |=  mCOUNTER_ENABLE; // start 

}

int main (){
	unsigned int iMainLoopCtr;
	Timer1Interrupts_Init(1000);

	while(1){
	 	iMainLoopCtr++;
	}
}
*/

/*2
#include <LPC21xx.H>
#include "led.h"

// Definicje dla Timera 0
#define mCOUNTER_ENABLE   (1 << 0)  // Wlaczenie licznika (bit 0 w T0TCR)
#define mCOUNTER_RESET    (1 << 1)  // Reset licznika (bit 1 w T0TCR)

// Definicje dla modulu por�wnania Timera 0
#define mINTERRUPT_ON_MR0 (1 << 0)  // Przerwanie przy dopasowaniu MR0 (bit 0 w T0MCR)
#define mRESET_ON_MR0     (1 << 1)  // Reset przy dopasowaniu MR0 (bit 1 w T0MCR)
#define mMR0_INTERRUPT    (1 << 0)  // Flaga przerwania MR0 (bit 0 w T0IR)

// Definicje dla VIC (Vector Interrupt Controller)
#define VIC_TIMER0_CHANNEL_NR 4     // Kanal 4 dla Timera 0
#define mIRQ_SLOT_ENABLE  (1 << 5)  // Wlaczenie slotu przerwania (bit 5 w VICVectCntl)

// Procedura obslugi przerwania Timera 0

//Bez __irq funkcja ISR moglaby zakl�cic dzialanie programu gl�wnego, np. przez nadpisywanie rejestr�w lub stosu.
//__irq automatyzuje wiele aspekt�w zwiazanych z obsluga przerwan, takich jak zapisywanie i przywracanie kontekstu, co ulatwia programowanie.


__irq void Timer0IRQHandler() {
    T0IR = mMR0_INTERRUPT;    // Wyczysc flage przerwania
    LedStepRight();           // Wykonaj akcje (przesuniecie diody w prawo)
    VICVectAddr = 0x00;       // Potwierdz obsluge przerwania
}

// Inicjalizacja przerwan Timera 0
void Timer0Interrupts_Init(unsigned int uiPeriod) { // Okres w milisekundach
    // Konfiguracja VIC
    VICIntEnable |= (1 << VIC_TIMER0_CHANNEL_NR);     // Wlacz kanal Timera 0
    VICVectCntl1  = mIRQ_SLOT_ENABLE | VIC_TIMER0_CHANNEL_NR; // Wlacz slot 1 dla Timera 0
    VICVectAddr1  = (unsigned long)Timer0IRQHandler;  // Ustaw adres ISR

    // Konfiguracja modulu por�wnania
    T0MR0 = 15 * uiPeriod;  // 15 000 cykli = 1 ms przy PCLK=15 MHz
    T0MCR |= (mINTERRUPT_ON_MR0 | mRESET_ON_MR0); // Przerwanie i reset na MR0

    // Wlaczenie timera
    T0TCR |= mCOUNTER_ENABLE;  // Uruchom timer
}

// Funkcja gl�wna
int main() {
    unsigned int iMainLoopCtr;
    Timer0Interrupts_Init(250000);  // Inicjalizacja z okresem 250 ms

    while (1) {
        iMainLoopCtr++;  // Petla nieskonczona
    }
}
*/

#include <LPC21xx.H>
#include "timer_interrupts.h"

// Definicje dla Timera 0
#define mCOUNTER_ENABLE   (1 << 0)
#define mCOUNTER_RESET    (1 << 1)

// Definicje dla modulu por�wnania Timera 0
#define mINTERRUPT_ON_MR0 (1 << 0)
#define mRESET_ON_MR0     (1 << 1)
#define mMR0_INTERRUPT    (1 << 0)

// Definicje dla VIC
#define VIC_TIMER0_CHANNEL_NR 4
#define mIRQ_SLOT_ENABLE  (1 << 5)

// Globalny wskaznik na funkcje
void (*ptrTimer0InterruptFunction)(void);

// Funkcja obslugi przerwania
__irq void Timer0IRQHandler(void) {
    T0IR = mMR0_INTERRUPT; // Wyczysc flage przerwania
		ptrTimer0InterruptFunction();
    VICVectAddr = 0x00; // Zakoncz obsluge przerwania
}

// Inicjalizacja timera z funkcja przerwania
void Timer0Interrupts_Init(unsigned int uiPeriod, void (*ptrInterruptFunction)(void)) {
	
    ptrTimer0InterruptFunction = ptrInterruptFunction; // Przypisz wskaznik na funkcje

    // Konfiguracja VIC
    VICIntEnable |= (1 << VIC_TIMER0_CHANNEL_NR);
    VICVectCntl1 = mIRQ_SLOT_ENABLE | VIC_TIMER0_CHANNEL_NR;
    VICVectAddr1 = (unsigned long)Timer0IRQHandler; //Typ (unsigned long) jest uzywany, poniewaz adres funkcji musi byc zapisany w formacie 32-bitowej liczby calkowitej bez znaku

    // Konfiguracja timera
    T0MR0 = 15 * uiPeriod; // uiPeriod w mikrosekundach, PCLK=15MHz
    T0MCR |= (mINTERRUPT_ON_MR0 | mRESET_ON_MR0);
    T0TCR |= mCOUNTER_ENABLE; // Wlacz timer
}

