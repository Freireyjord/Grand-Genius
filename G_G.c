#include <msp430.h>
#include <stdlib.h>

#define BUTr BIT4
#define BUTg BIT5
#define BUTb BIT6
#define BUTy BIT7

#define LEDr BIT0
#define LEDg BIT1
#define LEDb BIT2
#define LEDy BIT3

#define DELAY_CYCLES 100000

int sequence[100];
int sequence_length = 0;
int current_index = 0;
int game_over = 0;

void initialize()
{
    WDTCTL = WDTPW + WDTHOLD; // Desabilitar o Watchdog Timer
    P1DIR |= (LEDr + LEDg + LEDb + LEDy); // Definir pinos dos LEDs como saída
    P1OUT &= ~(LEDr + LEDg + LEDb + LEDy); // Desligar todos os LEDs
    P1REN |= (BUTr + BUTg + BUTb + BUTy); // Habilitar resistor de pull-up interno para os botões
    P1IE |= (BUTr + BUTg + BUTb + BUTy); // Habilitar interrupção para os botões
    P1IES |= (BUTr + BUTg + BUTb + BUTy); // Definir interrupção para borda de descida
    P1IFG &= ~(BUTr + BUTg + BUTb + BUTy); // Limpar as flags de interrupção dos botões
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
                P1OUT |= LEDr;
                __delay_cycles(DELAY_CYCLES);
                P1OUT &= ~LEDr;
                __delay_cycles(DELAY_CYCLES);
                break;
            case 1:
                P1OUT |= LEDg;
                __delay_cycles(DELAY_CYCLES);
                P1OUT &= ~LEDg;
                __delay_cycles(DELAY_CYCLES);
                break;
            case 2:
                P1OUT |= LEDb;
                __delay_cycles(DELAY_CYCLES);
                P1OUT &= ~LEDb;
                __delay_cycles(DELAY_CYCLES);
                break;
            case 3:
                P1OUT |= LEDy;
                __delay_cycles(DELAY_CYCLES);
                P1OUT &= ~LEDy;
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

void game_over_sequence()
{
    int i;
    for (i = 0; i < 3; i++) {
        P1OUT |= (LEDr + LEDg + LEDb + LEDy);
        __delay_cycles(DELAY_CYCLES);
        P1OUT &= ~(LEDr + LEDg + LEDb + LEDy);
        __delay_cycles(DELAY_CYCLES);
    }
    game_over = 1;
}

void button_press(int button)
{
    switch (button) {
        case 0:
            P1OUT |= LEDr;
            __delay_cycles(DELAY_CYCLES);
            P1OUT &= ~LEDr;
            __delay_cycles(DELAY_CYCLES);
            break;
        case 1:
            P1OUT |= LEDg;
            __delay_cycles(DELAY_CYCLES);
            P1OUT &= ~LEDg;
            __delay_cycles(DELAY_CYCLES);
            break;
        case 2:
            P1OUT |= LEDb;
            __delay_cycles(DELAY_CYCLES);
            P1OUT &= ~LEDb;
            __delay_cycles(DELAY_CYCLES);
            break;
        case 3:
            P1OUT |= LEDy;
            __delay_cycles(DELAY_CYCLES);
            P1OUT &= ~LEDy;
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
