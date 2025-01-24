#include "configbits.h" // Bits de configuration
#include <xc.h>         // Definition des registres specifiques au uC

void init_timer2(void) {
    T2CON = 0x07; // cr�er Timer2
    PR2 = 255; // Temps Timer2
    TMR2 = 0; // Timer2 = 0
    TMR2IF = 0; // flag stop
}

void delai_timer2(void) {

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
        for (int i = 0; i < 4; i++) {
            LATD = leds; // Allume la Led � allumer
            LATB = 0x00; // �teint tous le monde
            for (int i = 0; i < 10; i++) {
                TMR2IF = 0;
                while (!TMR2IF) {
                    //boucle infini
                }
            }
            leds <<= 1; // D�cale vers la Led � allumer d'un cran
        }

        leds = 0x01; // �teint tous le monde

        for (int i = 0; i < 4; i++) {
            LATD = 0x00; // �teint tous le monde
            LATB = leds; // Allume la Led � allumer
            for (int i = 0; i < 10; i++) {
                TMR2IF = 0;
                while (!TMR2IF) {
                    //boucle infini
                }
            }
            leds <<= 1; // D�cale vers la Led � allumer d'un cran
        }

        leds = 0x01; // �teint tous le monde
    }
}
