#include <msp430.h>
#include <stdlib.h>

// define o pino de saída dos LEDs
#define LEDy BIT0    
#define LEDr BIT1   
#define LEDb BIT2  
#define LEDg BIT3  

// define o pino de entrada do BUTTON
#define BUTy BIT5    
#define BUTb BIT4  
#define BUTr BIT1  
#define BUTg BIT0  

// Velocidade em que as LEDs piscam
#define DELAY_CYCLES 300000

// Variaveis do Genius
int sequence[100];
int sequence_length = 1;
int current_index = 0;
int Record = 0;


// Checa se os botões foram pressionados
int check_button_press1(int button)
{
    if (P1IFG & button) {
        return 0;
    } else {
        return 1;
    }
}
int check_button_press2(int button)
{
    if (P2IFG & button) {
        return 0;
    } else {
        return 1;
    }
}

// DERROTA
void game_over_sequence()
{
    int i;
    for (i = 0; i < 3; i++) {
        P1OUT |= (LEDr + LEDg + LEDb + LEDy);
        __delay_cycles(DELAY_CYCLES);
        P1OUT &= ~(LEDr + LEDg + LEDb + LEDy);
        __delay_cycles(DELAY_CYCLES);
    }
    if(Record < sequence_length)
        Record = sequence_length-1;
    sequence_length = 1;
    current_index = 0;
    next_sequence();
}

//Mostra a proxima sequencia
void next_sequence()
{
        for (int i = 0; i < sequence_length; i++) {
            sequence[i] = rand() % 4;
        }
        for(int j = 0; j < sequence_length;j++){
            switch (sequence[j]) {
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

int main(void) {

    WDTCTL = WDTPW | WDTHOLD;                          //WATCHDOG TIMER PARADO
    
    srand(1);                                         // Inicia os numeros aleatorios

        P1DIR |= (LEDr + LEDg + LEDb + LEDy);         // Definir pinos dos LEDs como saída
        P1OUT &= ~(LEDr + LEDg + LEDb + LEDy);        // Desligar todos os LEDs

        P1REN |= (BUTb + BUTy);                       // Habilitar resistor de pull-up interno para os botões
        P1IE |= (BUTb + BUTy);                        // Habilitar interrupção para os botões
        P1IES |= (BUTb + BUTy);                       // Definir interrupção para borda de descida
        P1IFG &= ~(BUTb + BUTy);                      // Limpar as flags de interrupção dos botões

        P2REN |= (BUTr + BUTg);                       // Habilitar resistor de pull-up interno para os botões
        P2IE |= (BUTr + BUTg);                        // Habilitar interrupção para os botões
        P2IES |= (BUTr + BUTg);                       // Definir interrupção para borda de descida
        P2IFG &= ~(BUTr + BUTg);                      // Limpar as flags de interrupção dos botões

        // Cria e mostra a primeira sequencia
        sequence[0] = rand() % 4;
        switch (sequence[0]) {
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

    __enable_interrupt();
}



#pragma vector = PORT1_VECTOR
__interrupt void port1_int(void){

    if (check_button_press1(BUTy)) {
        P1OUT |= LEDy;
        __delay_cycles(DELAY_CYCLES);
        P1OUT &= ~LEDy;
        __delay_cycles(DELAY_CYCLES);

        if(sequence[current_index] == 3){
            current_index += 1;
            if(current_index == sequence_length){
                current_index = 0;
                sequence_length += 1;
                next_sequence();
            }
        }else{
            game_over_sequence();
        }

    }
    if (check_button_press1(BUTb)) {
        P1OUT |= LEDb;
        __delay_cycles(DELAY_CYCLES);
        P1OUT &= ~LEDb;
        __delay_cycles(DELAY_CYCLES);

        if(sequence[current_index] == 2){
            current_index += 1;
            if(current_index == sequence_length){
                current_index = 0;
                sequence_length += 1;
                next_sequence();
            }
        }else{
            game_over_sequence();
        }

    }

    P1IFG &= ~(BUTb + BUTy);     //LIMPA FLAG DE INTERRUPÇÃO DO PINO P1.3
}

#pragma vector = PORT2_VECTOR
__interrupt void port2_int(void){

    if (check_button_press2(BUTr)) {
        P1OUT |= LEDr;
        __delay_cycles(DELAY_CYCLES);
        P1OUT &= ~LEDr;
        __delay_cycles(DELAY_CYCLES);
        if(sequence[current_index] == 0){
                    current_index += 1;
                    if(current_index == sequence_length){
                        current_index = 0;
                        sequence_length += 1;
                        next_sequence();
                    }
                }else{
                    game_over_sequence();
                }
    }
    if (check_button_press2(BUTg)) {
            P1OUT |= LEDg;
            __delay_cycles(DELAY_CYCLES);
            P1OUT &= ~LEDg;
            __delay_cycles(DELAY_CYCLES);
            if(sequence[current_index] == 1){
                        current_index += 1;
                        if(current_index == sequence_length){
                            current_index = 0;
                            sequence_length += 1;
                            next_sequence();
                        }
                    }else{
                        game_over_sequence();
                    }
        }

    P2IFG &= ~(BUTr + BUTg);     //LIMPA FLAG DE INTERRUPÇÃO DO PINO P1.3
}
