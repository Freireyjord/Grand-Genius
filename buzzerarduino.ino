#include <msp430.h>

#define BUZZER_PIN BIT2

void delay_us(unsigned int us) {
    while (us--) {
        __delay_cycles(1);
    }
}

void tone(unsigned int frequency, unsigned int duration) {
    unsigned int period = 1000000 / frequency;

    P1DIR |= BUZZER_PIN;
    TA0CCTL0 = CCIE;
    TA0CCR0 = period - 1;
    TA0CTL = TASSEL_2 + MC_1 + TACLR;

    delay_us(duration * 1000);

    TA0CTL = MC_0;
    P1DIR &= ~BUZZER_PIN;
}

void noTone() {
    TA0CTL = MC_0;
    P1DIR &= ~BUZZER_PIN;
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A(void) {
    P1OUT ^= BUZZER_PIN;
}

int main(void) {
    WDTCTL = WDTPW + WDTHOLD;

    while (1) {
        // Toca a melodia "Dó Ré Mi Fá Sol Lá Si Dó (oitava acima)"
        tone(261, 500);    // Dó
        tone(294, 500);    // Ré
        tone(329, 500);    // Mi
        tone(349, 500);    // Fá
        tone(392, 500);    // Sol
        tone(440, 500);    // Lá
        tone(493, 500);    // Si
        tone(523, 500);    // Dó (oitava acima)

        // Aguarda 1 segundo entre cada repetição da melodia
        __delay_cycles(1000000);
    }

    return 0;
}
