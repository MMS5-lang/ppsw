#include <LPC21xx.H>
#include "keyboard.h"

#define BUTT_S0 (1 << 4)
#define BUTT_S1 (1 << 6)
#define BUTT_S2 (1 << 5)
#define BUTT_S3 (1 << 7)

void KeyboardInit(void) {
    IO0DIR &= ~(BUTT_S0 | BUTT_S1 | BUTT_S2 | BUTT_S3);
}

enum KeyboardState eKeyboardRead(void) {
    if ((IO0PIN & BUTT_S0) == 0) {
        return BUTTON_0;
    } else if ((IO0PIN & BUTT_S1) == 0) {
        return BUTTON_1;
    } else if ((IO0PIN & BUTT_S2) == 0) {
        return BUTTON_2;
    } else if ((IO0PIN & BUTT_S3) == 0) {
        return BUTTON_3;
    } else {
        return RELASED;
    }
}
