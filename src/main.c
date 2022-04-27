#include "stm8s.h"

#include "milis.h"

#include "stm8_hd44780.h"

#include "delay.h"

#include <stdio.h>

#define _ISOC99_SOURCE
#define _GNU_SOURCE

void encoder_init(void){
    GPIO_Init(GPIOC, GPIO_PIN_1,GPIO_MODE_IN_PU_NO_IT);
    GPIO_Init(GPIOC, GPIO_PIN_2,GPIO_MODE_IN_PU_NO_IT);

    TIM1_DeInit();
    TIM1_TimeBaseInit(16, TIM1_COUNTERMODE_UP, 500, 16); //inicializace enkoderu (void encoder_init)
    TIM1_EncoderInterfaceConfig(TIM1_ENCODERMODE_TI12,TIM1_ICPOLARITY_FALLING,TIM1_ICPOLARITY_FALLING);
   TIM1_Cmd(ENABLE);
}

void setup(void)
{
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1); // taktovani MCU na 16MHz

    lcd_init(); //inicializace LCD displeje
    init_milis(); //inicializace milis

    GPIO_Init(GPIOC,GPIO_PIN_5,GPIO_MODE_OUT_PP_LOW_SLOW); // PC5 output

    encoder_init();

}

int main(void)
{
    uint32_t time = milis(); //inicializace main

    unsigned int present_value = 0x0000;
    unsigned int previous_value = 0x0001;

    char text[32];

    uint32_t perioda;

    setup();

    lcd_gotoxy(0,0);
    lcd_puts("perioda");
    while (1) {
        if(milis() - time > perioda/2){
            GPIO_WriteReverse(GPIOC,GPIO_PIN_5);
            time = milis();
        }


        present_value = TIM1_GetCounter();

        if(present_value != previous_value)
        {

            lcd_gotoxy(12, 0);
            sprintf(text,"%1u ",present_value*4);
            lcd_puts(text);
            perioda = present_value * 4;
        }
        previous_value = present_value;

    }
}

/*-------------------------------  Assert -----------------------------------*/
#include "__assert__.h"
