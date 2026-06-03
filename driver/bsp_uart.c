#include "bsp_uart.h"

#include "gd32a7xx.h"
#include <stdio.h>

#define BSP_UART1_PERIPH        LINFLEXD_UART1
#define BSP_UART1_CLK           RCU_LINFLEXD1
#define BSP_UART1_GPIO_CLK      RCU_GPIOB
#define BSP_UART1_GPIO_PORT     GPIOB
#define BSP_UART1_TX_PIN        GPIO_PIN_4
#define BSP_UART1_RX_PIN        GPIO_PIN_5
#define BSP_UART1_TX_AF         GPIO_AF_13
#define BSP_UART1_RX_AF         GPIO_AF_13

void bsp_uart1_init(uint32_t baudrate)
{
    linflexd_uart_parameter_struct initpara;

    if(0U == baudrate) {
        baudrate = BSP_UART1_DEFAULT_BAUDRATE;
    }

    rcu_periph_clock_enable(BSP_UART1_GPIO_CLK);
    rcu_periph_clock_enable(BSP_UART1_CLK);
    rcu_linflexd_clock_config(RCU_LINFLEXDSRC_CKSYS, 4U);

    gpio_af_set(BSP_UART1_GPIO_PORT, BSP_UART1_TX_AF, BSP_UART1_TX_PIN);
    gpio_af_set(BSP_UART1_GPIO_PORT, BSP_UART1_RX_AF, BSP_UART1_RX_PIN);
    gpio_mode_set(BSP_UART1_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, BSP_UART1_TX_PIN | BSP_UART1_RX_PIN);
    gpio_output_options_set(BSP_UART1_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_LEVEL_2, BSP_UART1_TX_PIN | BSP_UART1_RX_PIN);

    linflexd_uart_struct_para_init(&initpara);
    initpara.baudrate = baudrate;
    initpara.parityenable = DISABLE;
    initpara.stopbitscount = LINFLEXD_UART_ONE_STOP_BIT;
    initpara.wordlength = LINFLEXD_UART_8_BITS;
    initpara.txmodetype = LINFLEXD_UART_TXRX_BUFFER_MODE;
    initpara.rxmodetype = LINFLEXD_UART_TXRX_BUFFER_MODE;

    (void)linflexd_uart_init(BSP_UART1_PERIPH, &initpara);
    linflexd_uart_receiver_config(BSP_UART1_PERIPH, LINFLEXD_UART_RECEIVE_ENABLE);
    linflexd_uart_transmitter_config(BSP_UART1_PERIPH, LINFLEXD_UART_TRANSMIT_ENABLE);
}

void bsp_uart1_send_byte(uint8_t data)
{
    linflexd_uart_byte_transmit(BSP_UART1_PERIPH, data);
    while(RESET == linflexd_uart_flag_get(BSP_UART1_PERIPH, LINFLEXD_UART_FLAG_DTF_TFF)) {
    }
    linflexd_uart_flag_clear(BSP_UART1_PERIPH, LINFLEXD_UART_FLAG_DTF_TFF);
}

void bsp_uart1_send_string(const char *str)
{
    while((NULL != str) && ('\0' != *str)) {
        if('\n' == *str) {
            bsp_uart1_send_byte((uint8_t)'\r');
        }
        bsp_uart1_send_byte((uint8_t)*str);
        str++;
    }
}

int fputc(int ch, FILE *f)
{
    (void)f;
    bsp_uart1_send_byte((uint8_t)ch);
    return ch;
}
