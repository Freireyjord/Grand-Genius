#include <msp430.h>
#include <stdlib.h>

#define BUTTON1 BIT3
#define BUTTON2 BIT4
#define BUTTON3 BIT5
#define BUTTON4 BIT6

#define LED1 BIT0
#define LED2 BIT1
#define LED3 BIT2
#define LED4 BIT3

#define DELAY_CYCLES 100000

int sequence[100];
int sequence_length = 0;
int current_index = 0;
int game_over = 0;

void initialize()
{
    WDTCTL = WDTPW + WDTHOLD; // Desabilitar o Watchdog Timer
    P1DIR |= (LED1 + LED2 + LED3 + LED4); // Definir pinos dos LEDs como saída
    P1OUT &= ~(LED1 + LED2 + LED3 + LED4); // Desligar todos os LEDs
    P1REN |= (BUTTON1 + BUTTON2 + BUTTON3 + BUTTON4); // Habilitar resistor de pull-up interno para os botões
    P1IE |= (BUTTON1 + BUTTON2 + BUTTON3 + BUTTON4); // Habilitar interrupção para os botões
    P1IES |= (BUTTON1 + BUTTON2 + BUTTON3 + BUTTON4); // Definir interrupção para borda de descida
    P1IFG &= ~(BUTTON1 + BUTTON2 + BUTTON3 + BUTTON4); // Limpar as flags de interrupção dos botões
    __enable_interrupt(); // Habilitar interrupções gerais
}

void generate_next_sequence()
{
    int i;
    for (i = 0; i < sequence_length; i++) {
        sequence[i] = rand() % 4;
    }
}

void show_sequence()
{
    int i;
    for (i = 0; i < sequence_length; i++) {
        switch (sequence[i]) {
            case 0:
                P1OUT |= LED1;
                __delay_cycles(DELAY_CYCLES);
                P1OUT &= ~LED1;
                __delay_cycles(DELAY_CYCLES);
                break;
            case 1:
                P1OUT |= LED2;
                __delay_cycles(DELAY_CYCLES);
                P1OUT &= ~LED2;
                __delay_cycles(DELAY_CYCLES);
                break;
            case 2:
                P1OUT |= LED3;
                __delay_cycles(DELAY_CYCLES);
                P1OUT &= ~LED3;
                __delay_cycles(DELAY_CYCLES);
                break;
            case 3:
                P1OUT |= LED4;
                __delay_cycles(DELAY_CYCLES);
                P1OUT &= ~LED4;
                __delay_cycles(DELAY_CYCLES);
                break;
        }
    }
}

int check_button_press(int button)
{
    if (P1IN & button) {
        return 0;
    } else {
        return 1;
    }
}

void game_over_animation()
{
    int i;
    for (i = 0; i < 3; i++) {
        P1OUT |= (LED1 + LED2 + LED3 + LED4);
        __delay_cycles(DELAY_CYCLES);
        P1OUT &= ~(LED1 + LED2 + LED3 + LED4);
        __delay_cycles(DELAY_CYCLES);
    }
}

void game_over_sequence()
{
    game_over_animation();
    game_over = 1;
}

void button_press(int button)
{
    switch (button) {
        case 0:
            P1OUT |= LED1;
            __delay_cycles(DELAY_CYCLES);
            P1OUT &= ~LED1;
            __delay_cycles(DELAY_CYCLES);
            break;
        case 1:
            P1OUT |= LED2;
            __delay_cycles(DELAY_CYCLES);
            P1OUT &= ~LED2;
            __delay_cycles(DELAY_CYCLES);
            break;
        case 2:
            P1OUT |= LED3;
            __delay_cycles(DELAY_CYCLES);
            P1OUT &= ~LED3;
            __delay_cycles(DELAY_CYCLES);
            break;
        case 3:
            P1OUT |= LED4;
            __delay_cycles(DELAY_CYCLES);
            P1OUT &= ~LED4;
            __delay_cycles(DELAY_CYCLES);
            break;
    }
}

int main(void)
{
    initialize();
    srand(1); // Inicializar o gerador de números aleatórios

    while (1) {
        if (game_over) {
            __delay_cycles(DELAY_CYCLES * 5);
            sequence_length = 0;
            current_index = 0;
            game_over = 0;
            generate_next_sequence();
            show_sequence();
        } else {
            if (current_index == sequence_length) {
                sequence_length++;
                current_index = 0;
                show_sequence();
            }

            if (check_button_press(BUTTON1)) {
                button_press(0);
                if (sequence[current_index] != 0) {
                    game_over_sequence();
                } else {
                    current_index++;
                }
            } else if (check_button_press(BUTTON2)) {
                button_press(1);
                if (sequence[current_index] != 1) {
                    game_over_sequence();
                } else {
                    current_index++;
                }
            } else if (check_button_press(BUTTON3)) {
                button_press(2);
                if (sequence[current_index] != 2) {
                    game_over_sequence();
                } else {
                    current_index++;
                }
            } else if (check_button_press(BUTTON4)) {
                button_press(3);
                if (sequence[current_index] != 3) {
                    game_over_sequence();
                } else {
                    current_index++;
                }
            }
        }
    }
}
