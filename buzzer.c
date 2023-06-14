#include <msp430g2553.h>

#define BUZZER_PIN BIT0

void delay(int milliseconds) {
    int i;
    for (i = 0; i < milliseconds; i++) {
        __delay_cycles(1000); // Aproximadamente 1ms no MSP430 de 16MHz
    }
}

void buzzer_on() {
    P1OUT |= BUZZER_PIN; // Liga o buzzer
}

void buzzer_off() {
    P1OUT &= ~BUZZER_PIN; // Desliga o buzzer
}

void setup() {
    WDTCTL = WDTPW + WDTHOLD; // Desabilita o Watchdog Timer

    P1DIR |= BUZZER_PIN; // Define o pino do buzzer como saída

    buzzer_off(); // Desliga o buzzer inicialmente
}

int main(void) {
    setup();

    while (1) {
        buzzer_on(); // Liga o buzzer
        delay(1000); // Aguarda 1 segundo

        buzzer_off(); // Desliga o buzzer
        delay(1000); // Aguarda 1 segundo
    }

    return 0;
}
