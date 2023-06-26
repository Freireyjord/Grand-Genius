#include <msp430.h>
#include <stdlib.h>
#include "delay.h"
#include "SW_I2C.h"
#include "PCF8574.h"
#include "lcd.h"

// LCD

const unsigned char symbol[8] =
{
   0x00, 0x06, 0x09, 0x09, 0x06, 0x00, 0x00, 0x00
};
void GPIO_graceInit(void);
void BCSplus_graceInit(void);
void System_graceInit(void);
void WDTplus_graceInit(void);
void lcd_symbol(void);
void print_C(unsigned char x_pos, unsigned char y_pos, signed int value);
void print_I(unsigned char x_pos, unsigned char y_pos, signed long value);
void print_D(unsigned char x_pos, unsigned char y_pos, signed int value, unsigned char points);
void print_F(unsigned char x_pos, unsigned char y_pos, float value, unsigned char points);

// Define os pinos dos LEDs
#define LEDy BIT0
#define LEDr BIT1
#define LEDb BIT2
#define LEDg BIT3

// Define os pinos dos Botões
#define BUTy BIT5
#define BUTb BIT4
#define BUTr BIT1
#define BUTg BIT0

// SCL P1.6
// SDA P1.7

#define BUZZ BIT3 // Buzzer no pino P2.3

#define DELAY_CYCLES 1500000 // Tempo da LED piscar

// variaveis do jogo
int sequence[100];
int sequence_length = 1;
int current_index = 0;
int Record = 0;

// BUZZER
void apita() {
    P2OUT |= BUZZ; // Liga o buzzer
    __delay_cycles(100000);
    P2OUT &= ~BUZZ; // Desliga o buzzer
}

// Checagem de botão
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

// Gerador de sequência
void next_sequence()
{
    int i = 0;
        for (i = 0; i < sequence_length; i++) {
            sequence[i] = rand() % 4;
        }
        int j = 0;
        for(j = 0; j < sequence_length;j++){
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

// Sequência de derrota
void game_over_sequence()
{
    int i;
    for (i = 0; i < 3; i++) {
        P1OUT |= (LEDr + LEDg + LEDb + LEDy);
        __delay_cycles(DELAY_CYCLES);
        apita();
        P1OUT &= ~(LEDr + LEDg + LEDb + LEDy);
        __delay_cycles(DELAY_CYCLES);
    }
    if(Record < sequence_length){
        Record = sequence_length-1;
    }
    sequence_length = 1;
    current_index = 0;
    __delay_cycles(DELAY_CYCLES);
    next_sequence();
}



int main(void) {

    WDTCTL = WDTPW | WDTHOLD;       //WATCHDOG TIMER PARADO
    srand(1);

        P1DIR |= (LEDr + LEDg + LEDb + LEDy); // Definir pinos dos LEDs como saída
        P1OUT &= ~(LEDr + LEDg + LEDb + LEDy); // Desligar todos os LEDs

        P1REN |= (BUTb + BUTy); // Habilitar resistor de pull-up interno para os botões
        P1IE |= (BUTb + BUTy); // Habilitar interrupção para os botões
        P1IES |= (BUTb + BUTy); // Definir interrupção para borda de descida
        P1IFG &= ~(BUTb + BUTy); // Limpar as flags de interrupção dos botões

        P2REN |= (BUTr + BUTg); // Habilitar resistor de pull-up interno para os botões
        P2IE |= (BUTr + BUTg); // Habilitar interrupção para os botões
        P2IES |= (BUTr + BUTg); // Definir interrupção para borda de descida
        P2IFG &= ~(BUTr + BUTg); // Limpar as flags de interrupção dos botões

    // sequência inicial
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

    // LCD
        __enable_interrupt();

        GPIO_graceInit();

        BCSplus_graceInit();

        System_graceInit();

        WDTplus_graceInit();

        LCD_init();
        lcd_symbol();

        while(1)
        {
           LCD_goto(0, 0);
           LCD_putstr("Recorde:");
           print_C(12, 0, Record);
           LCD_goto(0, 1);
           LCD_putstr("Nivel:");
           print_C(12, 1, sequence_length);
           delay_ms(1000);
           P1IFG &= ~(BUTb + BUTy);     //LIMPA FLAG DE INTERRUPÇÃO DO PINO P1.3
           P2IFG &= ~(BUTr + BUTg);     //LIMPA FLAG DE INTERRUPÇÃO DO PINO P1.3
        }
}

// LCD
void GPIO_graceInit(void)
{

    P1SEL &= ~(BIT6 | BIT7);

}


void BCSplus_graceInit(void)
{

    BCSCTL2 = SELM_0 | DIVM_0 | DIVS_0;

    if (CALBC1_8MHZ != 0xFF)
    {
        __delay_cycles(100000);

        DCOCTL = 0x00;
        BCSCTL1 = CALBC1_8MHZ;
        DCOCTL = CALDCO_8MHZ;
    }

    BCSCTL1 |= XT2OFF | DIVA_0;

    BCSCTL3 = XT2S_0 | LFXT1S_0 | XCAP_1;

}


void System_graceInit(void)
{

    __bis_SR_register(GIE);

}


void WDTplus_graceInit(void)
{

    WDTCTL = WDTPW | WDTHOLD;

}


void lcd_symbol(void)
{
    unsigned char s = 0;

   LCD_send(0x40, CMD);

   for(s = 0; s < 8; s++)
   {
        LCD_send(symbol[s], DAT);
   }

   LCD_send(0x80, CMD);
}


void print_C(unsigned char x_pos, unsigned char y_pos, signed int value)
{
     char ch[5] = {0x20, 0x20, 0x20, 0x20, '\0'};

     if(value < 0x00)
     {
        ch[0] = 0x2D;
        value = -value;
     }
     else
     {
        ch[0] = 0x20;
     }

     if((value > 99) && (value <= 999))
     {
         ch[1] = ((value / 100) + 0x30);
         ch[2] = (((value % 100) / 10) + 0x30);
         ch[3] = ((value % 10) + 0x30);
     }
     else if((value > 9) && (value <= 99))
     {
         ch[1] = (((value % 100) / 10) + 0x30);
         ch[2] = ((value % 10) + 0x30);
         ch[3] = 0x20;
     }
     else if((value >= 0) && (value <= 9))
     {
         ch[1] = ((value % 10) + 0x30);
         ch[2] = 0x20;
         ch[3] = 0x20;
     }

     LCD_goto(x_pos, y_pos);
     LCD_putstr(ch);
}


void print_I(unsigned char x_pos, unsigned char y_pos, signed long value)
{
    char ch[7] = {0x20, 0x20, 0x20, 0x20, 0x20, 0x20, '\0'};

    if(value < 0)
    {
        ch[0] = 0x2D;
        value = -value;
    }
    else
    {
        ch[0] = 0x20;
    }

    if(value > 9999)
    {
        ch[1] = ((value / 10000) + 0x30);
        ch[2] = (((value % 10000)/ 1000) + 0x30);
        ch[3] = (((value % 1000) / 100) + 0x30);
        ch[4] = (((value % 100) / 10) + 0x30);
        ch[5] = ((value % 10) + 0x30);
    }

    else if((value > 999) && (value <= 9999))
    {
        ch[1] = (((value % 10000)/ 1000) + 0x30);
        ch[2] = (((value % 1000) / 100) + 0x30);
        ch[3] = (((value % 100) / 10) + 0x30);
        ch[4] = ((value % 10) + 0x30);
        ch[5] = 0x20;
    }
    else if((value > 99) && (value <= 999))
    {
        ch[1] = (((value % 1000) / 100) + 0x30);
        ch[2] = (((value % 100) / 10) + 0x30);
        ch[3] = ((value % 10) + 0x30);
        ch[4] = 0x20;
        ch[5] = 0x20;
    }
    else if((value > 9) && (value <= 99))
    {
        ch[1] = (((value % 100) / 10) + 0x30);
        ch[2] = ((value % 10) + 0x30);
        ch[3] = 0x20;
        ch[4] = 0x20;
        ch[5] = 0x20;
    }
    else
    {
        ch[1] = ((value % 10) + 0x30);
        ch[2] = 0x20;
        ch[3] = 0x20;
        ch[4] = 0x20;
        ch[5] = 0x20;
    }

    LCD_goto(x_pos, y_pos);
    LCD_putstr(ch);
}

void print_D(unsigned char x_pos, unsigned char y_pos, signed int value, unsigned char points)
{
    char ch[5] = {0x2E, 0x20, 0x20, '\0'};

    ch[1] = ((value / 100) + 0x30);

    if(points > 1)
    {
        ch[2] = (((value / 10) % 10) + 0x30);

        if(points > 1)
        {
            ch[3] = ((value % 10) + 0x30);
        }
    }

    LCD_goto(x_pos, y_pos);
    LCD_putstr(ch);
}


void print_F(unsigned char x_pos, unsigned char y_pos, float value, unsigned char points)
{
    signed long tmp = 0x0000;

    tmp = value;
    print_I(x_pos, y_pos, tmp);
    tmp = ((value - tmp) * 1000);

    if(tmp < 0)
    {
       tmp = -tmp;
    }

    if(value < 0)
    {
        value = -value;
        LCD_goto(x_pos, y_pos);
        LCD_putchar(0x2D);
    }
    else
    {
        LCD_goto(x_pos, y_pos);
        LCD_putchar(0x20);
    }

    if((value >= 10000) && (value < 100000))
    {
        print_D((x_pos + 6), y_pos, tmp, points);
    }
    else if((value >= 1000) && (value < 10000))
    {
        print_D((x_pos + 5), y_pos, tmp, points);
    }
    else if((value >= 100) && (value < 1000))
    {
        print_D((x_pos + 4), y_pos, tmp, points);
    }
    else if((value >= 10) && (value < 100))
    {
        print_D((x_pos + 3), y_pos, tmp, points);
    }
    else if(value < 10)
    {
        print_D((x_pos + 2), y_pos, tmp, points);
    }
}


// 
#pragma vector = PORT1_VECTOR
__interrupt void port1_int(void){
    if (check_button_press1(BUTy)) {
        P1OUT |= LEDy;
        __delay_cycles(DELAY_CYCLES);
        P1OUT &= ~LEDy;
        __delay_cycles(DELAY_CYCLES);

        if(sequence[current_index] == 3){
            apita();
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
            apita();
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
    print_C(12, 1, sequence_length);
}

#pragma vector = PORT2_VECTOR
__interrupt void port2_int(void){

    if (check_button_press2(BUTr)) {
        P1OUT |= LEDr;
        __delay_cycles(DELAY_CYCLES);
        P1OUT &= ~LEDr;
        __delay_cycles(DELAY_CYCLES);
        if(sequence[current_index] == 0){
            apita();
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
                apita();
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
    print_C(12, 1, sequence_length);
}
