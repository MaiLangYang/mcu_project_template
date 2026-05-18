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

#include "gd32a7xx.h"
#ifdef GD32A711X_A712X
//#include "gd32a712_evb.h"
#else
//#include "gd32a7x4_evb.h"
#endif /* GD32A711X_A712X */
//#include "systick.h"
#include "main.h"
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
    /************************************************************/
    /* Enabling the parameter checking feature of the firmware  */
    /* will reduce code execution efficiency. It is recommended */
    /* that the parameter checking feature be used only during  */
    /* the code development phase.                              */
    /************************************************************/
//#ifdef __FIRMWARE_VERSION_DEFINE
//    uint32_t fw_ver = 0;
//#endif
//    cache_enable();
//    /* configure systick */
//    systick_config();
//    /* initialize the LEDs, USART and key */
//    gd_eval_led_init(LED1);
//    gd_eval_led_init(LED2);
//    gd_eval_key_init(KEY_1, KEY_MODE_GPIO);
//    gd_eval_com_init(EVAL_COMA);
//    gd_eval_led_on(LED1);
//    gd_eval_led_on(LED2);
//    /* print out the clock frequency of system, AHB, APB1 and APB2 */
//    printf("\r\nCK_SYS is %d", rcu_clock_freq_get(CK_SYS));
//    printf("\r\nCK_AHB is %d", rcu_clock_freq_get(CK_AHB));
//    printf("\r\nCK_APB1 is %d", rcu_clock_freq_get(CK_APB1));
//    printf("\r\nCK_APB2 is %d", rcu_clock_freq_get(CK_APB2));
//#ifdef __FIRMWARE_VERSION_DEFINE
//    fw_ver = gd32a7xx_firmware_version_get();
//    /* print firmware version */
//    printf("\r\nGD32A7xxx series firmware version: V%d.%d.%d", (uint8_t)(fw_ver >> 24), (uint8_t)(fw_ver >> 16), (uint8_t)(fw_ver >> 8));
//#endif /* __FIRMWARE_VERSION_DEFINE */
    while(1) {
//        if(SET == gd_eval_key_state_get(KEY_1)) {
//            delay_1ms(50);
//            if(SET == gd_eval_key_state_get(KEY_1)) {
//                gd_eval_led_toggle(LED2);
//            }
//            while(SET == gd_eval_key_state_get(KEY_1)) {
//            }
//        }
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

