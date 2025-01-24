#include "configbits.h" // Bits de configuration
#include <xc.h>         // Definition des registres specifiques au uC

void main(void) {
    /* Code d'initialisation */
    TRISD = 0x00; //port D
    TRISB = 0x00; //port B

    LATD = 0x00; // D1-D4 éteintes
    LATB = 0x00; // D5-D8 éteintes

    while (1) {
        /* Code a executer dans une boucle infinie */
        LATD = 0xFF; //D1-D4 allumées
        LATB = 0x00; //D5-D8 éteintes
        for (unsigned int i = 0; i < 20000; i++) {
            NOP(); // attends
        }

        LATD = 0x00; //D1-D4 éteintes
        LATB = 0xFF; //D5-D8 allumées
        for (unsigned int i = 0; i < 20000; i++) {
            NOP(); //attends
        }
    }
}