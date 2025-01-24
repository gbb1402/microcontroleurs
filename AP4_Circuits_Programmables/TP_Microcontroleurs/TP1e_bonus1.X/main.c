#include "configbits.h" // Bits de configuration
#include <xc.h>         // Definition des registres specifiques au uC

volatile unsigned char leds_timer2 = 0x08;
volatile int timer2_flag = 0;

void __interrupt() isr(void) {
    if (TMR2IF) {
        TMR2IF = 0;
        leds_timer2 >>= 1;
        if (leds_timer2 == 0x00) {
            leds_timer2 = 0x08;
        }
        timer2_flag = 1;
    }
}

void init_timer2(void) {
    T2CON = 0x07;
    PR2 = 255;
    TMR2 = 0;
    TMR2IF = 0;
    TMR2IE = 1;
    PEIE = 1;
    GIE = 1;
}

void main(void) {
    /* Code d'initialisation */
    TRISD = 0x00; //port D
    TRISB = 0x00; //port B

    LATD = 0x00; // D1-D4 éteintes
    LATB = 0x00; // D5-D8 éteintes

    unsigned char leds_manual = 0x80; // Commence avec la LED D8

    init_timer2(); // Initialise Timer2

    while (1) {
        /* Code a executer dans une boucle infinie */
        if (timer2_flag) {
            LATD = leds_timer2;
            timer2_flag = 0;
        }

        LATB = leds_manual;
        for (unsigned int i = 0; i < 20000; i++) {
            NOP(); // attends
        }
        leds_manual >>= 1; // Décale les LEDs vers la droite
        if (leds_manual == 0x00) {
            leds_manual = 0x80;
        }
    }
}
