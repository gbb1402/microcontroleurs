#include "configbits.h" // Bits de configuration
#include <xc.h>         // Definition des registres specifiques au uC

volatile unsigned char leds_timer2 = 0x08; // Variable qui contrôle les LEDs via Timer2, commence avec D4 allumée
volatile int timer2_flag = 0; // Drapeau d'interruption pour synchroniser l'affichage des LEDs

void __interrupt() isr(void) { // Fonction d'interruption
    if (TMR2IF) { // Vérifie si l'interruption provient du Timer2
        TMR2IF = 0; // Efface le drapeau d'interruption de Timer2
        leds_timer2 >>= 1; // Décale les LEDs vers la droite
        if (leds_timer2 == 0x00) { 
            leds_timer2 = 0x08; // Réinitialise la séquence après 4 décalages (revient à D4)
        }
        timer2_flag = 1;  // Indique au programme principal qu'une mise à jour est nécessaire
    }
}

void init_timer2(void) {
    T2CON = 0x07; // Active Timer2 avec un prescaler de 1:16
    PR2 = 255; // Définit la période du Timer2
    TMR2 = 0; // Réinitialise le compteur Timer2
    TMR2IF = 0; // Efface le drapeau d'interruption du Timer2
    TMR2IE = 1; // Active l'interruption du Timer2
    PEIE = 1; // Active les interruptions des périphériques
    GIE = 1; // Active les interruptions globales
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
        if (timer2_flag) { // Vérifie si une interruption Timer2 s'est produite
            LATD = leds_timer2; // Met à jour l'affichage des LEDs sur PORTD
            timer2_flag = 0; // Réinitialise le drapeau d'interruption
        }

        LATB = leds_manual; // Met à jour l'affichage des LEDs sur PORTB
        for (unsigned int i = 0; i < 20000; i++) { 
            NOP(); // attends
        }
        leds_manual >>= 1; // Décale les LEDs vers la droite
        if (leds_manual == 0x00) {
            leds_manual = 0x80; // Réinitialise la séquence après avoir atteint D5
        }
    }
}