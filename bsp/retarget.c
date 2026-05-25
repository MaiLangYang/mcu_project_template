/*!
    \file    retarget.c
    \brief   printf retarget to board USART
*/

#include "drv_usart.h"
#include <stdio.h>

int fputc(int ch, FILE *f)
{
    (void)f;
    drv_usart_send_byte((uint8_t)ch);
    return ch;
}
