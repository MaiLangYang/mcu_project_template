/*!
    \file    drv_usart.h
    \brief   board USART driver
*/

#ifndef DRV_USART_H
#define DRV_USART_H

#include "board_config.h"
#include <stdint.h>

#define DRV_USART_RX_BUFFER_SIZE       128U

void drv_usart_init(uint32_t baudrate);
void drv_usart_send_byte(uint8_t data);
void drv_usart_send_buffer(const uint8_t *data, uint32_t length);
void drv_usart_send_string(const char *str);
uint32_t drv_usart_read(uint8_t *data, uint32_t length);
uint32_t drv_usart_rx_available(void);
void drv_usart_irq_handler(void);

#endif /* DRV_USART_H */
