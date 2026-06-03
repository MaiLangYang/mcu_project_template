#include "bsp_led.h"

typedef struct {
    uint32_t port;
    uint32_t pin;
    rcu_periph_enum clk;
} bsp_led_hw_t;

static const bsp_led_hw_t led_hw[BSP_LED_COUNT] = {
    {GPIOC, GPIO_PIN_2, RCU_GPIOC},
    {GPIOC, GPIO_PIN_3, RCU_GPIOC},
    {GPIOA, GPIO_PIN_0, RCU_GPIOA},
};

static uint8_t led_valid(bsp_led_t led)
{
    return ((uint32_t)led < (uint32_t)BSP_LED_COUNT);
}

void bsp_led_init(void)
{
    uint32_t i;

    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOC);

    for(i = 0U; i < (uint32_t)BSP_LED_COUNT; i++) {
        gpio_mode_set(led_hw[i].port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, led_hw[i].pin);
        gpio_output_options_set(led_hw[i].port, GPIO_OTYPE_PP, GPIO_OSPEED_LEVEL_2, led_hw[i].pin);
        gpio_bit_reset(led_hw[i].port, led_hw[i].pin);
    }
}

void bsp_led_on(bsp_led_t led)
{
    if(led_valid(led)) {
        gpio_bit_set(led_hw[led].port, led_hw[led].pin);
    }
}

void bsp_led_off(bsp_led_t led)
{
    if(led_valid(led)) {
        gpio_bit_reset(led_hw[led].port, led_hw[led].pin);
    }
}

void bsp_led_toggle(bsp_led_t led)
{
    if(led_valid(led)) {
        gpio_bit_toggle(led_hw[led].port, led_hw[led].pin);
    }
}

void bsp_led_write(bsp_led_t led, FlagStatus state)
{
    if(SET == state) {
        bsp_led_on(led);
    } else {
        bsp_led_off(led);
    }
}
