/*!
    \file    board_config.h
    \brief   board-level configuration for GD32A503CCT3 template
*/

#ifndef BOARD_CONFIG_H
#define BOARD_CONFIG_H

#include "gd32a50x.h"

#define BOARD_HXTAL_VALUE              8000000U

/* USART0: PB13 -> TX, PB14 -> RX */
#define BOARD_USART                    USART0
#define BOARD_USART_RCU                RCU_USART0
#define BOARD_USART_GPIO_RCU           RCU_GPIOB
#define BOARD_USART_TX_PORT            GPIOB
#define BOARD_USART_TX_PIN             GPIO_PIN_13
#define BOARD_USART_RX_PORT            GPIOB
#define BOARD_USART_RX_PIN             GPIO_PIN_14
#define BOARD_USART_GPIO_AF            GPIO_AF_5
#define BOARD_USART_IRQn               USART0_IRQn
#define BOARD_USART_BAUDRATE           115200U

/* I2C1: PE10 -> SCL, PE11 -> SDA */
#define BOARD_I2C                      I2C1
#define BOARD_I2C_RCU                  RCU_I2C1
#define BOARD_I2C_GPIO_RCU             RCU_GPIOE
#define BOARD_I2C_SCL_PORT             GPIOE
#define BOARD_I2C_SCL_PIN              GPIO_PIN_10
#define BOARD_I2C_SDA_PORT             GPIOE
#define BOARD_I2C_SDA_PIN              GPIO_PIN_11
#define BOARD_I2C_GPIO_AF              GPIO_AF_5
#define BOARD_I2C_TIMEOUT              100000U

/* I2C timing values follow the official I2C1 examples for GD32A50x. */
#define BOARD_I2C_TIMING_PSC           0x0U
#define BOARD_I2C_TIMING_SCL_DELAY     0x5U
#define BOARD_I2C_TIMING_SDA_DELAY     0x0U
#define BOARD_I2C_TIMING_SCL_HIGH      0x1DU
#define BOARD_I2C_TIMING_SCL_LOW       0x59U

/* ADC0: PA3 -> ADC0_IN11 */
#define BOARD_ADC                      ADC0
#define BOARD_ADC_RCU                  RCU_ADC0
#define BOARD_ADC_GPIO_RCU             RCU_GPIOA
#define BOARD_ADC_PORT                 GPIOA
#define BOARD_ADC_PIN                  GPIO_PIN_3
#define BOARD_ADC_CHANNEL              ADC_CHANNEL_11
#define BOARD_ADC_SAMPLE_TIME          ADC_SAMPLETIME_55POINT5

#endif /* BOARD_CONFIG_H */
