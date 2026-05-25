# Pin Configuration

All board-specific peripheral choices are defined in:

```text
bsp/board_config.h
```

## USART0

```c
#define BOARD_USART                    USART0
#define BOARD_USART_TX_PORT            GPIOB
#define BOARD_USART_TX_PIN             GPIO_PIN_13
#define BOARD_USART_RX_PORT            GPIOB
#define BOARD_USART_RX_PIN             GPIO_PIN_14
#define BOARD_USART_GPIO_AF            GPIO_AF_5
```

## I2C1

```c
#define BOARD_I2C                      I2C1
#define BOARD_I2C_SCL_PORT             GPIOE
#define BOARD_I2C_SCL_PIN              GPIO_PIN_10
#define BOARD_I2C_SDA_PORT             GPIOE
#define BOARD_I2C_SDA_PIN              GPIO_PIN_11
#define BOARD_I2C_GPIO_AF              GPIO_AF_5
```

## ADC0

```c
#define BOARD_ADC                      ADC0
#define BOARD_ADC_PORT                 GPIOA
#define BOARD_ADC_PIN                  GPIO_PIN_3
#define BOARD_ADC_CHANNEL              ADC_CHANNEL_11
```
