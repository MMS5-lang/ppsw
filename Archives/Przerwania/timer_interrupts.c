#include <LPC21xx.H> 
#include "timer_interrupts.h"

// Definicje masek bitowych dla Timera 0
#define mCOUNTER_ENABLE   (1 << 0)
#define mCOUNTER_RESET    (1 << 1)

// Definicje masek bitowych dla modulu porównania Timera 0 (Match Control Register)
#define mINTERRUPT_ON_MR0 (1 << 0)
#define mRESET_ON_MR0     (1 << 1)
#define mMR0_INTERRUPT    (1 << 0) 

// Definicje dla VIC (Vector Interrupt Controller)
#define VIC_TIMER0_CHANNEL_NR 4    // Kanal 4 w VIC odpowiada Timer0 
#define mIRQ_SLOT_ENABLE  (1 << 5) // Bit 5 w VICVectCntl: wlacza slot przerwania 

// Globalny wskaznik na funkcje, która bedzie wywolywana w przerwaniu
void (*ptrTimer0InterruptFunction)(void);

// Procedura obslugi przerwania (ISR - Interrupt Service Routine) Timera 0
// __irq oznacza, ze jest to ISR; kompilator generuje odpowiedni kod wejscia/wyjscia dla trybu IRQ
//Jest to procedura obslugi przerwania (ISR – Interrupt Service Routine), która automatycznie wykonuje sie, gdy Timer 0 zglosi przerwanie
//Ta funkcja dziala jako "most" miedzy sprzetem (Timer 0 i VIC) a oprogramowaniem (funkcja uzytkownika)
__irq void Timer0IRQHandler(void) {
    T0IR = mMR0_INTERRUPT; // Czysci flage przerwania MR0 w Timer0 (zapis 1 do bitu 0 w T0IR, str. 217)
                           // Bez tego VIC nie wiedzialby, ze przerwanie zostalo obsluzone
    ptrTimer0InterruptFunction(); // Wywoluje funkcje uzytkownika przypisana do przerwania
                                  // Umozliwia dynamiczna zmiane akcji w przerwaniu
    VICVectAddr = 0x00; // Potwierdza zakonczenie obslugi przerwania w VIC (zapis 0 do VICVectAddr)
                        // Informuje VIC, ze ISR sie zakonczylo, umozliwiajac obsluge kolejnych przerwan
}

// Funkcja inicjalizujaca przerwania Timera 0
// Przyjmuje okres (w mikrosekundach) i wskaznik na funkcje do wywolania w przerwaniu
void Timer0Interrupts_Init(unsigned int uiPeriod, void (*ptrInterruptFunction)(void)) {
    // Przypisanie funkcji uzytkownika do globalnego wskaznika
    ptrTimer0InterruptFunction = ptrInterruptFunction; // Funkcja ptrInterruptFunction bedzie wywolywana w ISR

    // Konfiguracja VIC (Vector Interrupt Controller)
    VICIntEnable |= (0x1 << VIC_TIMER0_CHANNEL_NR); // Wlacza kanal 4 (Timer0) w VIC
                                                    // Umozliwia VIC odbieranie przerwan z Timera 0
    VICVectCntl1 = mIRQ_SLOT_ENABLE | VIC_TIMER0_CHANNEL_NR; // Konfiguruje slot 1 w VIC:
                                                             // - Bit 5 (mIRQ_SLOT_ENABLE) wlacza slot
                                                             // - Bity 0-4 (VIC_TIMER0_CHANNEL_NR) przypisuja kanal 4 (Timer0)
    VICVectAddr1 = (unsigned long)Timer0IRQHandler; // Ustawia adres procedury ISR (Timer0IRQHandler) dla slotu 1
                                                    // VIC bedzie skakal do tej procedury przy przerwaniu z Timera 0

    // Konfiguracja Timera 0
    T0MR0 = (15 * uiPeriod); // Ustawia wartosc w rejestrze Match Register 0 (T0MR0)
                             // uiPeriod w mikrosekundach; PCLK = 15 MHz, wiec 1 µs = 15 cykli
                             // Przykladowo, dla uiPeriod = 1000 µs, T0MR0 = 15 * 1000 = 15 000 cykli (1 ms)
    T0MCR |= (mINTERRUPT_ON_MR0 | mRESET_ON_MR0); // Konfiguruje Match Control Register (T0MCR):
                                                  // - mINTERRUPT_ON_MR0: generuje przerwanie po osiagnieciu T0MR0
                                                  // - mRESET_ON_MR0: resetuje licznik po osiagnieciu T0MR0
                                                  // (str. 218 manuala)
    T0TCR |= mCOUNTER_ENABLE; // Wlacza zliczanie cykli w Timer0 (bit 0 w T0TCR, str. 217)
                              // Timer zaczyna zliczac cykle zegara PCLK, generujac przerwania co zadany okres
}


/*
Wejscie (IntEnable): Na poczatku VIC ma uklad, który pozwala wlaczac lub wylaczac przerwania z poszczególnych kanalów (1-wlaczone 0-wylaczone)
Sloty (VectorInterrupt0-15): VIC ma 16 "slotów" – to takie miejsca, gdzie mozna przypisac konkretne przerwania i powiedziec, co z nimi robic
	Kazdy slot ma:
	Bit Enable (w rejestrze VectorCtl): Wlacza lub wylacza slot - 5 bit
	Bity Source (tez w VectorCtl): Okreslaja, który kanal (od 0 do 31) ten slot obsluguje.
	Adres procedury (VectorAddr): Miejsce, gdzie zapisany jest adres programu (ISR), który ma sie uruchomic, gdy przerwanie z tego kanalu sie pojawi, inaczej priorytet.
	
	Przyklad:
		1.Jakies zródlo (np. TIMER0 na kanale 4) wywoluje przerwanie.
		2.VIC sprawdza, czy kanal 4 jest wlaczony w IntEnable.
		3.Szuka wsród 16 slotów, czy którys jest przypisany do kanalu 4 i wlaczony.
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

// Definicje dla modulu porównania Timera 0
#define mINTERRUPT_ON_MR0 (1 << 0)  // Przerwanie przy dopasowaniu MR0 (bit 0 w T0MCR)
#define mRESET_ON_MR0     (1 << 1)  // Reset przy dopasowaniu MR0 (bit 1 w T0MCR)
#define mMR0_INTERRUPT    (1 << 0)  // Flaga przerwania MR0 (bit 0 w T0IR)

// Definicje dla VIC (Vector Interrupt Controller)
#define VIC_TIMER0_CHANNEL_NR 4     // Kanal 4 dla Timera 0
#define mIRQ_SLOT_ENABLE  (1 << 5)  // Wlaczenie slotu przerwania (bit 5 w VICVectCntl)

// Procedura obslugi przerwania Timera 0

//Bez __irq funkcja ISR moglaby zaklócic dzialanie programu glównego, np. przez nadpisywanie rejestrów lub stosu.
//__irq automatyzuje wiele aspektów zwiazanych z obsluga przerwan, takich jak zapisywanie i przywracanie kontekstu, co ulatwia programowanie.


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

    // Konfiguracja modulu porównania
    T0MR0 = 15 * uiPeriod;  // 15 000 cykli = 1 ms przy PCLK=15 MHz
    T0MCR |= (mINTERRUPT_ON_MR0 | mRESET_ON_MR0); // Przerwanie i reset na MR0

    // Wlaczenie timera
    T0TCR |= mCOUNTER_ENABLE;  // Uruchom timer
}

// Funkcja glówna
int main() {
    unsigned int iMainLoopCtr;
    Timer0Interrupts_Init(250000);  // Inicjalizacja z okresem 250 ms

    while (1) {
        iMainLoopCtr++;  // Petla nieskonczona
    }
}
*/



