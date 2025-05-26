#include "led.h"
#include "keyboard.h"
#include "timer.h"

/* test 1
int main(void) {

    InitTimer0();
    LedInit();
    
    while (1) {

        LedOn(1);
        WaitOnTimer0(500000); //pol sekundy
        LedOn(4);
        WaitOnTimer0(500000);
    }
    
}
*/

//test drugiego cwiczenia
int main(void) {
    
    InitTimer0Match0(500000); // Inicjalizacja Timera0 z opóznieniem 500 ms (500000 µs) przy uzyciu bloku porównujacego
    InitTimer0();
    LedInit();    
    
    while (1) {

        LedOn(2);
        WaitOnTimer0Match0();  // Czekaj 500 ms
        
				LedOn(4);
        WaitOnTimer0Match0();  // Czekaj 500 ms
    }
}

