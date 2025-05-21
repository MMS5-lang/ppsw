#include <LPC21xx.H>

#define LED0_bm 0x10000
#define LED1_bm 0x20000
#define LED2_bm 0x40000
#define LED3_bm 0x80000

#define BUTTON_S0_bm 0x10
#define BUTTON_S1_bm 0x40
#define BUTTON_S2_bm 0x20
#define BUTTON_S3_bm 0x80

enum eKeyboardState {RELASED,BUTTON_0,BUTTON_1,BUTTON_2,BUTTON_3};
enum eDirection {LEFT, RIGHT};

void Delay(unsigned int uiMilliSeconds){
	
	unsigned int uiSecondsLimit;
	unsigned int uiHelpCounter = uiMilliSeconds *12000;
	
	for(uiSecondsLimit = 0 ; uiSecondsLimit<uiHelpCounter ; uiSecondsLimit++){}
}

void LedInit(){
	IO1DIR = IO1DIR | (LED0_bm | LED1_bm | LED2_bm | LED3_bm);
	IO1SET=LED0_bm;
}



void LedOn(unsigned char ucLedIndex) {
	
	IO1CLR = (LED0_bm | LED1_bm | LED2_bm | LED3_bm);
	
	switch(ucLedIndex){
		case 0:
			IO1SET = LED0_bm;
			break;
		case 1:
			IO1SET = LED1_bm;
			break;
		case 2:
			IO1SET = LED2_bm;
			break;
		case 3:
			IO1SET = LED3_bm;
		default:
			break;
	}
}
    
		
void KeyboardInit() {
    IO0DIR = IO0DIR & ~(BUTTON_S0_bm | BUTTON_S1_bm | BUTTON_S2_bm | BUTTON_S3_bm);
}

enum eKeyboardState eKeyboardRead(){
	if((BUTTON_S0_bm & IO0PIN)==0){
		return BUTTON_0;
	}
	else if ((BUTTON_S1_bm & IO0PIN)==0){ //  0x40   0x0
		return BUTTON_1;
	}
	else if ((BUTTON_S2_bm & IO0PIN)==0){ //  0x20   0x0
		return BUTTON_2;
	}
	else if ((BUTTON_S3_bm & IO0PIN)==0){
		return BUTTON_3;
	}
	else{
		return RELASED;
	}
}

void LedStep(enum eDirection eDirection) {
	
		static unsigned int uiCurrentLedIndex = 0;
	
    if (eDirection == LEFT) {
        uiCurrentLedIndex = (uiCurrentLedIndex + 1);
    } else {
        uiCurrentLedIndex = (uiCurrentLedIndex - 1); //uc 0 odejmujemy 1 i ile bedzie
    }
    LedOn(uiCurrentLedIndex % 4);
}

void LedStepLeft(void) {
    LedStep(LEFT);
}


void LedStepRight(void) {
    LedStep(RIGHT);
}

int main(){

/*
	LedInit();
	KeyboardInit();
	while (1) {
				enum eKeyboardState eKeyboardState;
        eKeyboardState = eKeyboardRead();
        switch (eKeyboardState) {
            case BUTTON_0:
                LedOn(0);
                break;
            case BUTTON_1:
                LedOn(1); 
                break;
            case BUTTON_2:
                LedOn(2); 
                break;
            case BUTTON_3:
                LedOn(3); 
                break;
            case RELASED:
                LedOn(4); 
                break;
        }
    }
*/
/*
		unsigned char ucStepCount = 0;
		Delay(1000);
    LedInit();
    KeyboardInit();

    while (1) {
        for (ucStepCount = 0; ucStepCount < 9; ucStepCount++) {
            LedStep(RIGHT);
            Delay(300);
        }
        for (ucStepCount = 0; ucStepCount < 9; ucStepCount++) {
            LedStep(LEFT);
            Delay(300);
        }
    }

*/
/*
	enum eKeyboardState eKeyboardState;

    LedInit();
    KeyboardInit();

    while (1) {
        eKeyboardState = eKeyboardRead();
        switch (eKeyboardState) {
            case BUTTON_1:
                LedStep(RIGHT);
                break;
            case BUTTON_2:
                LedStep(LEFT);
                break;
            case RELASED:
                break;
        }
        Delay(200);
    }	
*/
}


