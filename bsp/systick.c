/*!
    \file    systick.c
    \brief   SysTick delay implementation
*/

#include "gd32a50x.h"
#include "systick.h"

static volatile uint32_t delay_count;

void systick_config(void)
{
    if(SysTick_Config(SystemCoreClock / 1000U)) {
        while(1) {
        }
    }
    NVIC_SetPriority(SysTick_IRQn, 0x00U);
}

void delay_1ms(uint32_t count)
{
    delay_count = count;
    while(0U != delay_count) {
    }
}

void delay_decrement(void)
{
    if(0U != delay_count) {
        delay_count--;
    }
}
