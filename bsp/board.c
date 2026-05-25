/*!
    \file    board.c
    \brief   board initialization
*/

#include "board.h"
#include "drv_adc.h"
#include "drv_i2c.h"
#include "drv_usart.h"
#include "systick.h"

void board_init(void)
{
    systick_config();
    drv_usart_init(BOARD_USART_BAUDRATE);
    drv_i2c_init();
    drv_adc_init();
}
