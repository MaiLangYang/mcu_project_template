/*!
    \file    drv_usart.c
    \brief   board USART driver
*/

#include "drv_usart.h"
#include <stddef.h>

static volatile uint8_t rx_buffer[DRV_USART_RX_BUFFER_SIZE];
static volatile uint16_t rx_write_index;
static volatile uint16_t rx_read_index;

static uint16_t rx_next_index(uint16_t index)
{
    index++;
    if(index >= DRV_USART_RX_BUFFER_SIZE) {
        index = 0U;
    }
    return index;
}

void drv_usart_init(uint32_t baudrate)
{
    rcu_periph_clock_enable(BOARD_USART_GPIO_RCU);
    rcu_periph_clock_enable(BOARD_USART_RCU);
    rcu_usart_clock_config(BOARD_USART, RCU_USARTSRC_CKSYS);

    gpio_af_set(BOARD_USART_TX_PORT, BOARD_USART_GPIO_AF, BOARD_USART_TX_PIN);
    gpio_af_set(BOARD_USART_RX_PORT, BOARD_USART_GPIO_AF, BOARD_USART_RX_PIN);

    gpio_mode_set(BOARD_USART_TX_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, BOARD_USART_TX_PIN);
    gpio_output_options_set(BOARD_USART_TX_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, BOARD_USART_TX_PIN);

    gpio_mode_set(BOARD_USART_RX_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, BOARD_USART_RX_PIN);
    gpio_output_options_set(BOARD_USART_RX_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, BOARD_USART_RX_PIN);

    usart_deinit(BOARD_USART);
    usart_word_length_set(BOARD_USART, USART_WL_8BIT);
    usart_stop_bit_set(BOARD_USART, USART_STB_1BIT);
    usart_parity_config(BOARD_USART, USART_PM_NONE);
    usart_baudrate_set(BOARD_USART, baudrate);
    usart_receive_config(BOARD_USART, USART_RECEIVE_ENABLE);
    usart_transmit_config(BOARD_USART, USART_TRANSMIT_ENABLE);

    rx_write_index = 0U;
    rx_read_index = 0U;
    nvic_irq_enable(BOARD_USART_IRQn, 1U, 0U);
    usart_interrupt_enable(BOARD_USART, USART_INT_RBNE);
    usart_enable(BOARD_USART);
}

void drv_usart_send_byte(uint8_t data)
{
    usart_data_transmit(BOARD_USART, data);
    while(RESET == usart_flag_get(BOARD_USART, USART_FLAG_TBE)) {
    }
}

void drv_usart_send_buffer(const uint8_t *data, uint32_t length)
{
    uint32_t i;

    if(NULL == data) {
        return;
    }

    for(i = 0U; i < length; i++) {
        drv_usart_send_byte(data[i]);
    }
}

void drv_usart_send_string(const char *str)
{
    if(NULL == str) {
        return;
    }

    while('\0' != *str) {
        drv_usart_send_byte((uint8_t)*str);
        str++;
    }
}

uint32_t drv_usart_rx_available(void)
{
    if(rx_write_index >= rx_read_index) {
        return (uint32_t)(rx_write_index - rx_read_index);
    }

    return (uint32_t)(DRV_USART_RX_BUFFER_SIZE - rx_read_index + rx_write_index);
}

uint32_t drv_usart_read(uint8_t *data, uint32_t length)
{
    uint32_t count = 0U;

    if(NULL == data) {
        return 0U;
    }

    while((count < length) && (rx_read_index != rx_write_index)) {
        data[count++] = rx_buffer[rx_read_index];
        rx_read_index = rx_next_index(rx_read_index);
    }

    return count;
}

void drv_usart_irq_handler(void)
{
    uint16_t next_index;

    if(RESET != usart_interrupt_flag_get(BOARD_USART, USART_INT_FLAG_RBNE)) {
        next_index = rx_next_index(rx_write_index);
        if(next_index != rx_read_index) {
            rx_buffer[rx_write_index] = (uint8_t)usart_data_receive(BOARD_USART);
            rx_write_index = next_index;
        } else {
            (void)usart_data_receive(BOARD_USART);
        }
    }
}
