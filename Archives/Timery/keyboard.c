#include <LPC21xx.H>
#include "keyboard.h"
#define BUTTON_S0_bm (1 << 4)
#define BUTTON_S1_bm (1 << 6)
#define BUTTON_S2_bm (1 << 5)
#define BUTTON_S3_bm (1 << 7)



void KeyboardInit() {
    IO0DIR = IO0DIR & ~(BUTTON_S0_bm | BUTTON_S1_bm | BUTTON_S2_bm | BUTTON_S3_bm);
}

enum eKeyboardState eKeyboardRead(){
    if((IO0PIN & BUTTON_S0_bm)==0){
        return BUTTON_0;
    }
    else if ((IO0PIN & BUTTON_S1_bm)==0){
        return BUTTON_1;
    }
    else if ((IO0PIN & BUTTON_S2_bm)==0){
        return BUTTON_2;
    }
    else if ((IO0PIN & BUTTON_S3_bm)==0){
        return BUTTON_3;
    }
    else{
        return RELASED;
    }
}
