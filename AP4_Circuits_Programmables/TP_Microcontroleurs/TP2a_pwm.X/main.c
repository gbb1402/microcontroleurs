#include <xc.h>

#define _XTAL_FREQ 8000000  // Fr�quence du microcontr�leur

// D�finition des ports et boutons
#define LED_TRIS  TRISD0  // LED sur RD0 (Sortie PWM)
#define LED_LAT   LATD0   // Contr�le de la LED via PWM
#define LED_PORT  RD0PPS  // Attribution PPS pour PWM4

#define BOUTON_TRIS  TRISB0         // Bouton sur RB0
#define BOUTON_PORT  PORTBbits.RB0  // Lecture du bouton

// Param�tres du PWM
#define PWM_PRESCALER 0b10  // Prescaler 16
#define PWM_POSTSCALER 0b0000  // Postscaler 1:1
#define PWM_PERIOD 199  // PR2 = 199 pour obtenir 1 kHz

// D�finition des duty cycles en valeurs enti�res
//DutyCycleRatio�4�(PR2+1)
#define DUTY_CYCLE_LOW  ((0.1 * 4 * (PWM_PERIOD + 1)))   // 10% du cycle de travail
#define DUTY_CYCLE_HIGH ((1 * 4 * (PWM_PERIOD + 1)))  // 100% du cycle de travail

// Initialisation des ports et du bouton
void init_ports(void) {
    TRISD = 0x00;  // PORTD en sortie
    TRISB = 0x00;  // PORTB en sortie
    LATD = 0x00;   // �teindre LEDs sur PORTD
    LATB = 0x00;   // �teindre LEDs sur PORTB

    TRISBbits.BOUTON_TRIS = 1; // PORTB0 en entr�e
    ANSELB = 0x00;             // D�sactiver les entr�es analogiques
}

// Initialisation de la PWM4
void init_pwm4(void) {
    T2CONbits.T2CKPS = PWM_PRESCALER;    // Prescaler d�fini
    T2CONbits.T2OUTPS = PWM_POSTSCALER;  // Postscaler d�fini
    PR2 = PWM_PERIOD;                    // P�riode de la PWM

    PWM4CONbits.PWM4EN = 1;
    LED_PORT = 0x0F;  // Attribution PPS pour RD0 (Sortie PWM)
    T2CONbits.TMR2ON = 1;  // D�marrer Timer 2
}

// Fonction pour ajuster le duty cycle du PWM
void set_pwm_duty_cycle(unsigned int pwm_value) {
    PWM4DCH = (pwm_value >> 2) & 0xFF;  // Stockage des 8 bits de poids fort 
    PWM4DCL = (pwm_value << 6) & 0xC0;  // Stockage des 2 bits de poids faible
}

// Mise � jour du PWM selon l'�tat du bouton
void update_pwm_duty_cycle(void) {
    set_pwm_duty_cycle((BOUTON_PORT == 0) ? DUTY_CYCLE_HIGH : DUTY_CYCLE_LOW);
}

// Fonction de configuration initiale
void setup(void) {
    init_ports();
    init_pwm4();
}

// Boucle principale
void main(void) {
    setup();
    while (1) {
        update_pwm_duty_cycle();  // Ajuster la luminosit� de la LED en fonction du bouton
    }
}
