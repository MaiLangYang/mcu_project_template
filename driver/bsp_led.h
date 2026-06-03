#ifndef BSP_LED_H
#define BSP_LED_H

#include "gd32a7xx.h"

typedef enum {
    BSP_LED0 = 0,
    BSP_LED1,
    BSP_LED2,
    BSP_LED_COUNT
} bsp_led_t;

void bsp_led_init(void);
void bsp_led_on(bsp_led_t led);
void bsp_led_off(bsp_led_t led);
void bsp_led_toggle(bsp_led_t led);
void bsp_led_write(bsp_led_t led, FlagStatus state);

#endif /* BSP_LED_H */
