#include <LPC21xx.H>

#include "timer_interrupts.h"
#include "led.h"
#include "servo.h"

#define DETECTOR_bm (1 << 10)

// Definiuje stany automatu stanowego dla serwomechanizmu
enum ServoState {CALLIB, IDLE, IN_PROGRESS};
// Definiuje stany detektora
enum DetectorState {ACTIVE, INACTIVE};

// Inicjalizuje detektor, konfigurujac pin P0.10 jako wejscie
void DetectorInit(void){
    IO0DIR = IO0DIR & (~DETECTOR_bm); 
}

enum DetectorState eReadDetector(void){
    if((IO0PIN & DETECTOR_bm) == 0){  // Sprawdza, czy bit 10 w rejestrze IO0PIN jest 0 (niski stan = marker nad detektorem)
        return ACTIVE;  // Zwraca ACTIVE
    }else{
        return INACTIVE;  // Zwraca INACTIVE
    }
}

// Definiuje strukture do przechowywania stanu i pozycji serwomechanizmu
struct Servo{
    enum ServoState eState;  // Aktualny stan automatu serwomechanizmu (CALLIB, IDLE, IN_PROGRESS)
    unsigned int uiCurrentPosition;  // Aktualna pozycja serwomechanizmu (np. liczba kroków od pozycji zerowej)
    unsigned int uiDesiredPosition;  // Docelowa pozycja serwomechanizmu, do której ma dazyc
};
struct Servo sServo;// Deklaruje globalna zmienna sServo do zarzadzania stanem serwomechanizmu

// Automat stanowy sterujacy serwomechanizmem
void Automat(void){
    switch(sServo.eState) {  // Wybiera dzialanie na podstawie aktualnego stanu automatu
        case CALLIB:  // Stan kalibracji - ustala pozycje zerowa serwomechanizmu
            if(eReadDetector() == INACTIVE){ 
                LedStepLeft();  // Wykonuje krok w lewo (obrotu serwomechanizmu w przeciwna strone wskazowek zegara)
            }else{ 
                sServo.uiCurrentPosition = 0;  // Ustawia aktualna pozycje na 0
                sServo.uiDesiredPosition = 0;  // Ustawia docelowa pozycje na 0
                sServo.eState = IDLE;  // Przechodzi do stanu bezczynnosci (IDLE)
            }
            break;
        
        case IDLE:  // Stan bezczynnosci
            if(sServo.uiCurrentPosition != sServo.uiDesiredPosition){ 
                sServo.eState = IN_PROGRESS; 
            }else{ 
                sServo.eState = IDLE; 
            }
            break;
        
        case IN_PROGRESS:  // Stan ruchu - serwomechanizm dazy do docelowej pozycji
            if(sServo.uiCurrentPosition < sServo.uiDesiredPosition){ 
                LedStepRight();
                sServo.uiCurrentPosition++; 
            }else if(sServo.uiCurrentPosition > sServo.uiDesiredPosition){ 
               LedStepLeft();
                sServo.uiCurrentPosition--;
            }else{ 
                sServo.eState = IDLE; 
            }
            break;
    }
}

// Inicjalizuje serwomechanizm, ustawiajac poczatkowe parametry
void ServoInit(unsigned int uiFrequency){
    LedInit(); 
    DetectorInit(); 
    Timer0Interrupts_Init((1000000/uiFrequency), &Automat);  // Konfiguruje przerwania timera z okresem w mikrosekundach (1000000/freq) i funkcja Automat jako obsluga przerwania
    sServo.eState = CALLIB;  // Ustawia stan poczatkowy na CALLIB, aby rozpoczac kalibracje od rozruchu
}

// Rozpoczyna proces kalibracji serwomechanizmu
void ServoCallib(void){
    sServo.eState = CALLIB;  // Ustawia stan automatu na CALLIB, co uruchamia proces kalibracji w funkcji Automacie
}

// Ustawia docelowa pozycje serwomechanizmu
void ServoGoTo(unsigned int uiPosition){
    sServo.uiDesiredPosition = uiPosition;
}
