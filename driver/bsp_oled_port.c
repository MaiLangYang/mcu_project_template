#include "bsp_oled_port.h"

#define BSP_OLED_GPIO_CLK    RCU_GPIOJ
#define BSP_OLED_GPIO_PORT   GPIOJ
#define BSP_OLED_SCL_PIN     GPIO_PIN_10
#define BSP_OLED_SDA_PIN     GPIO_PIN_11

void bsp_oled_port_init(void)
{
    rcu_periph_clock_enable(BSP_OLED_GPIO_CLK);

    gpio_bit_set(BSP_OLED_GPIO_PORT, BSP_OLED_SCL_PIN | BSP_OLED_SDA_PIN);
    gpio_mode_set(BSP_OLED_GPIO_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, BSP_OLED_SCL_PIN | BSP_OLED_SDA_PIN);
    gpio_output_options_set(BSP_OLED_GPIO_PORT, GPIO_OTYPE_OD, GPIO_OSPEED_LEVEL_2, BSP_OLED_SCL_PIN | BSP_OLED_SDA_PIN);
}

void bsp_oled_scl_high(void)
{
    gpio_bit_set(BSP_OLED_GPIO_PORT, BSP_OLED_SCL_PIN);
}

void bsp_oled_scl_low(void)
{
    gpio_bit_reset(BSP_OLED_GPIO_PORT, BSP_OLED_SCL_PIN);
}

void bsp_oled_sda_high(void)
{
    gpio_bit_set(BSP_OLED_GPIO_PORT, BSP_OLED_SDA_PIN);
}

void bsp_oled_sda_low(void)
{
    gpio_bit_reset(BSP_OLED_GPIO_PORT, BSP_OLED_SDA_PIN);
}

FlagStatus bsp_oled_sda_read(void)
{
    return gpio_input_bit_get(BSP_OLED_GPIO_PORT, BSP_OLED_SDA_PIN);
}
