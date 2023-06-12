#include <msp430g2553.h>

#define BUZZER_PIN BIT2

void tone(unsigned int frequency, unsigned int duration) {
    unsigned int period = 1000000 / frequency;

    P1DIR |= BUZZER_PIN;
    TACCR0 = period - 1;
    TACCTL0 = CCIE;
    TACTL = TASSEL_2 + MC_1 + TACLR;

    __delay_cycles(duration * 1000);

    TACTL = MC_0;
    P1DIR &= ~BUZZER_PIN;
}

void noTone() {
    TACTL = MC_0;
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
