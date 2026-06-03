#ifndef BSP_UART_H
#define BSP_UART_H

#include <stdint.h>

#define BSP_UART1_DEFAULT_BAUDRATE    (115200U)

void bsp_uart1_init(uint32_t baudrate);
void bsp_uart1_send_byte(uint8_t data);
void bsp_uart1_send_string(const char *str);

#endif /* BSP_UART_H */
