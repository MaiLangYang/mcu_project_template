/*!
    \file    app.c
    \brief   application example
*/

#include "app.h"
#include "drv_adc.h"
#include "drv_usart.h"
#include "systick.h"
#include <stdio.h>

void app_init(void)
{
    printf("\r\nGD32A503CCT3 template start\r\n");
    printf("USART0: PB13/PB14, I2C1: PE10/PE11, ADC0_IN11: PA3\r\n");
}

void app_loop(void)
{
    uint8_t rx_data[16];
    uint32_t rx_len;
    uint16_t adc_value;

    adc_value = drv_adc_read_board_channel();
    printf("ADC0_IN11 = %u\r\n", adc_value);

    rx_len = drv_usart_read(rx_data, sizeof(rx_data));
    if(0U != rx_len) {
        drv_usart_send_buffer(rx_data, rx_len);
    }

    delay_1ms(1000U);
}
