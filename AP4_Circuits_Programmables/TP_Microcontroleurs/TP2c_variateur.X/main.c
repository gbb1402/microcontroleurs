#include <xc.h>

#define _XTAL_FREQ 8000000  // Fr�quence du microcontr�leur

// D�finition des ports et du PWM
#define LED_TRIS  TRISD0  // LED sur RD0 (Sortie PWM)
#define LED_LAT   LATD0   // Contr�le de la LED via PWM
#define LED_PORT  RD0PPS  // Attribution PPS pour PWM4

#define POTENTIOMETRE_TRIS  TRISA0  // Potentiom�tre sur RA0
#define POTENTIOMETRE_ANSEL  ANSELA0  // Configuration en entr�e analogique

// Param�tres du PWM
#define PWM_PRESCALER 0b10  // Prescaler 16
#define PWM_POSTSCALER 0b0000  // Postscaler 1:1
#define PWM_PERIOD 199  // PR2 = 255 pour 8-bit PWM

// Initialisation des ports et de l'ADC
void init_ports(void) {
    TRISD = 0x00;  // PORTD en sortie
    LATD = 0x00;   // �teindre LEDs sur PORTD
    
    // Configuration du potentiom�tre en entr�e analogique
    TRISA = 0xFF; // PORTA en entr�e
    ANSELA = 0xFF; // RA0 en mode analogique
}

// Initialisation de la PWM4
void init_pwm4(void) {
    T2CONbits.T2CKPS = PWM_PRESCALER;    // Prescaler d�fini
    T2CONbits.T2OUTPS = PWM_POSTSCALER;  // Postscaler d�fini
    PR2 = PWM_PERIOD;                    // P�riode de la PWM

    PWM4CONbits.PWM4EN = 1;  // Activer PWM
    LED_PORT = 0x0F;  // Attribution PPS pour RD0 (Sortie PWM)
    T2CONbits.TMR2ON = 1;  // D�marrer Timer 2
}

// Fonction pour configurer l'ADC
void init_ADC(void) {
    ADCON1 = 0x80; // ADC activ�, justification � droite, VREF+ = VDD
    ADCON0 = 0x01; // S�lection du canal AN0 (RA0)
}

// Lecture de l'ADC (potentiom�tre)
unsigned int read_ADC(void) {
    ADCON0bits.GO = 1;  // D�marrer la conversion ADC
    while (ADCON0bits.GO); // Attendre la fin
    return ((ADRESH << 8) + ADRESL); // R�cup�rer la valeur 10 bits (0-1023)
}

// Fonction pour ajuster le duty cycle du PWM par paliers de 10%
void set_pwm_duty_cycle(unsigned int adc_value) {
    unsigned int palier = adc_value / 102; // Divise en 10 paliers (0-9)
    unsigned int pwm_value = (palier * PWM_PERIOD) / 10; // �chelle 0-255 en pas de 10%

    PWM4DCH = (pwm_value >> 2) & 0xFF;  // Stockage des 8 bits de poids fort
    PWM4DCL = (pwm_value << 6) & 0xC0;  // Stockage des 2 bits de poids faible
}

// Boucle principale
void main(void) {
    init_ports();
    init_pwm4();
    init_ADC();

    while (1) {
        unsigned int adc_value = read_ADC();  // Lire valeur ADC
        set_pwm_duty_cycle(adc_value);  // Ajuster PWM en fonction du potentiom�tre (par paliers)
    }
}
