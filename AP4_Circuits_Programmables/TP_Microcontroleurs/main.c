#include <xc.h>

#define _XTAL_FREQ 8000000  // Fréquence du microcontrôleur

// Configuration des registres ADC
void init_ADC(void) {
    ADCON1 = 0x80; // ADC activé, justification à droite
    ADCON0 = 0x01; // Sélection du canal AN0 (RA0)
}

// Lecture de l'ADC (potentiomètre)
unsigned int read_ADC(void) {
    ADCON0bits.GO = 1;  // Démarrer la conversion ADC
    while (ADCON0bits.GO); // Attendre la fin
    return ((ADRESH << 8) + ADRESL); // Récupérer la valeur 10 bits
}

// Initialisation des ports
void init_ports(void) {
    TRISD = 0x00; // LEDs D1-D4 en sortie
    TRISB = 0x00; // LEDs D5-D8 en sortie
    TRISA = 0xFF; // Port A en entrée (potentiomètre)
    ANSELA = 0xFF; // Configurer RA0 en entrée analogique
}

// Fonction pour allumer une seule LED en fonction du potentiomètre
void update_LEDs(void) {
    unsigned int adc_value = read_ADC(); // Lire la valeur ADC
    unsigned char led_index = (adc_value * 8) / 1024; // Mapper 0-1023 en 0-7
    
    LATD = 0x00; // Éteindre LEDs D1-D4
    LATB = 0x00; // Éteindre LEDs D5-D8

    // Allumer la LED correspondante
    if (led_index < 4) {
        LATD = (1 << led_index);
    } else {
        LATB = (1 << (led_index - 4));
    }
}

// Boucle principale
void main(void) {
    init_ports();
    init_ADC();

    while (1) {
        update_LEDs();
    }
}
