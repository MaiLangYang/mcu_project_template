/*!
    \file    main.c
    \brief   led spark with systick, USART print and key example

    \version 2026-1-23, V1.1.0, firmware for GD32A7xx
*/

/*
    Copyright (c) 2026, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software without
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/

#include <stdio.h>
#include "gd32a7xx.h"
#include "bsp_adc_calib.h"
#include "bsp_key.h"
#include "bsp_led.h"
#include "bsp_motor.h"
#include "bsp_uart.h"
#include "main.h"
#include "oled.h"
#include "systick.h"

void cache_enable(void);

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/

int main(void)
{
    cache_enable();
    systick_config();

    bsp_led_init();
    bsp_key_init();
    bsp_uart1_init(BSP_UART1_DEFAULT_BAUDRATE);
    OLED_Init();
    bsp_motor_init();
    bsp_adc_calib_init();

    printf("\r\nGD32A712AVT3 template start");
    printf("\r\nCK_SYS  = %lu", (unsigned long)rcu_clock_freq_get(CK_SYS));
    printf("\r\nCK_AHB  = %lu", (unsigned long)rcu_clock_freq_get(CK_AHB));
    printf("\r\nCK_APB1 = %lu", (unsigned long)rcu_clock_freq_get(CK_APB1));
    printf("\r\nCK_APB2 = %lu\r\n", (unsigned long)rcu_clock_freq_get(CK_APB2));
    printf("seq,raw,mv,avg_mv\r\n");

    OLED_ShowString(0U, 0U, "GD32A712", 16U, 1U);
    OLED_ShowString(0U, 16U, "OLED OK", 16U, 1U);
    OLED_Refresh();

    while(1) {
        bsp_led_write(BSP_LED0, bsp_key_read(BSP_KEY1));
        bsp_led_write(BSP_LED1, bsp_key_read(BSP_KEY2));
        bsp_led_write(BSP_LED2, bsp_key_read(BSP_KEY3));

        if(SET == bsp_key_read(BSP_KEY4)) {
            bsp_motor_forward(5000U);
        } else {
            bsp_motor_stop();
        }

        bsp_adc_calib_poll();
        delay_1ms(10U);
    }
}

/*!
    \brief      enable the CPU cache
    \param[in]  none
    \param[out] none
    \retval     none
*/
void cache_enable(void)
{
    /* enable i-cache */
    SCB_EnableICache();
    /* enable d-cache */
    SCB_EnableDCache();
}

/*!
    \brief      toggle the led every 500ms
    \param[in]  none
    \param[out] none
    \retval     none
*/
void led_spark(void)
{
//    static __IO uint32_t timingdelaylocal = 0U;

//    timingdelaylocal++;
//    if(timingdelaylocal >= 500U) {
//        gd_eval_led_toggle(LED1);
//        timingdelaylocal = 0U;
//    }
}
