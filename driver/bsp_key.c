#include "bsp_key.h"

typedef struct {
    uint32_t port;
    uint32_t pin;
    rcu_periph_enum clk;
} bsp_key_hw_t;

static const bsp_key_hw_t key_hw[BSP_KEY_COUNT] = {
    {GPIOE, GPIO_PIN_13, RCU_GPIOE},
    {GPIOE, GPIO_PIN_15, RCU_GPIOE},
    {GPIOB, GPIO_PIN_11, RCU_GPIOB},
    {GPIOJ, GPIO_PIN_5, RCU_GPIOJ},
};

static uint8_t key_valid(bsp_key_t key)
{
    return ((uint32_t)key < (uint32_t)BSP_KEY_COUNT);
}

void bsp_key_init(void)
{
    uint32_t i;

    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOE);
    rcu_periph_clock_enable(RCU_GPIOJ);

    for(i = 0U; i < (uint32_t)BSP_KEY_COUNT; i++) {
        gpio_mode_set(key_hw[i].port, GPIO_MODE_INPUT, GPIO_PUPD_NONE, key_hw[i].pin);
    }
}

FlagStatus bsp_key_read(bsp_key_t key)
{
    if(key_valid(key)) {
        return gpio_input_bit_get(key_hw[key].port, key_hw[key].pin);
    }

    return RESET;
}
