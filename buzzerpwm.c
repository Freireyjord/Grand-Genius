#include <msp430g2553.h>

#define BUZZER_PIN BIT2

void tone(unsigned int frequency, unsigned int duration) {
    unsigned int period = 1000000 / frequency;

    P1DIR |= BUZZER_PIN;    // Configura o pino do buzzer como saída
    TACCR0 = period - 1;    // Define o valor de comparação do temporizador para controlar a frequência do som
    TACCTL0 = CCIE;         // Habilita a interrupção de comparação do temporizador
    TACTL = TASSEL_2 + MC_1 + TACLR;    // Configura o Timer A com clock SMCLK, modo de contagem UP e limpa o contador

    __delay_cycles(duration * 1000);    // Aguarda a duração desejada do som

    TACTL = MC_0;           // Para a contagem do Timer A
    P1DIR &= ~BUZZER_PIN;   // Configura o pino do buzzer como entrada (desligado)
}

void noTone() {
    TACTL = MC_0;           // Para a contagem do Timer A
    P1DIR &= ~BUZZER_PIN;   // Configura o pino do buzzer como entrada (desligado)
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A(void) {
    P1OUT ^= BUZZER_PIN;    // Alterna o estado do pino do buzzer para gerar a onda sonora
}

int main(void) {
    WDTCTL = WDTPW + WDTHOLD;   // Para o watchdog timer

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

