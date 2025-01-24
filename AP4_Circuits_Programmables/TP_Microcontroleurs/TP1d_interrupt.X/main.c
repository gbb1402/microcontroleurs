#include "configbits.h" // Bits de configuration
#include <xc.h>         // Definition des registres specifiques au uC

volatile unsigned char leds_timer2 = 0x01;
volatile unsigned char port_flag = 0;

void __interrupt() isr(void) {
    if (TMR2IF) {
        TMR2IF = 0;

        if (port_flag == 0) {
            LATD = leds_timer2;
            LATB = 0x00;
            leds_timer2 <<= 1;
            if (leds_timer2 == 0x10) {
                leds_timer2 = 0x01;
                port_flag = 1;
            }
        } else {
            LATB = leds_timer2;
            LATD = 0x00;
            leds_timer2 <<= 1;
            if (leds_timer2 == 0x10) {
                leds_timer2 = 0x01;
                port_flag = 0;
            }
        }
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

    T2CONbits.T2OUTPS = 0b1111;
}

void main(void) {
    /* Code d'initialisation */
    TRISD = 0x00; //port D
    TRISB = 0x00; //port B

    LATD = 0x00; // D1-D4 éteintes
    LATB = 0x00; // D5-D8 éteintes

    init_timer2(); // Initialise Timer2

    while (1) {
        /* Code a executer dans une boucle infinie */
    }
}
