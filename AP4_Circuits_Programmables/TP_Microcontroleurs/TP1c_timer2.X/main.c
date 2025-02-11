#include "configbits.h" // Bits de configuration
#include <xc.h>         // Definition des registres specifiques au uC

void init_timer2(void) {
    T2CONbits.T2OUTPS = 0b1111; // cr�er Timer2
    T2CONbits.T2CKPS = 0b00; // prescaler
    PR2 = 124; // Temps Timer2
    TMR2 = 0; // Timer2 = 0
    TMR2IF = 0; // flag stop
    PIR1bits.TMR2IF = 0; // Efface le flag d'interruption
    T2CONbits.TMR2ON = 1; // Active Timer2
}

void main(void) {
    /* Code d'initialisation */
    TRISD = 0x00; //port D
    TRISB = 0x00; //port B

    LATD = 0x00; // D1-D4 �teintes
    LATB = 0x00; // D5-D8 �teintes

    unsigned char leds = 0x01; // Led � allumer

    init_timer2();

    while (1) {
        /* Code a executer dans une boucle infinie */
        leds = 0x01; // �teint tous le monde
        for (int i = 0; i < 4; i++) {
            LATD = leds; // Allume la Led � allumer
            LATB = 0x00; // �teint tous le monde
            for (int i = 0; i < 125; i++) {
                PIR1bits.TMR2IF = 0; // Efface le flag d'interruption
                while (!PIR1bits.TMR2IF); // Attent 1ms
            }
            leds <<= 1; // D�cale vers la Led � allumer d'un cran
        }

        leds = 0x01; // �teint tous le monde

        for (int i = 0; i < 4; i++) {
            LATD = 0x00; // �teint tous le monde
            LATB = leds; // Allume la Led � allumer
            for (int i = 0; i < 125; i++) {
                PIR1bits.TMR2IF = 0; // Efface le flag d'interruption
                while (!PIR1bits.TMR2IF); // Attent 1ms
            }
            leds <<= 1; // D�cale vers la Led � allumer d'un cran
        }

        leds = 0x01; // �teint tous le monde
    }
}
