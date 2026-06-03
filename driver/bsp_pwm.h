#ifndef BSP_PWM_H
#define BSP_PWM_H

#include <stdint.h>

#define BSP_PWM_DUTY_MAX        (10000U)
#define BSP_PWM_DEFAULT_FREQ_HZ (20000U)

typedef enum {
    BSP_PWM_CH1 = 0,
    BSP_PWM_CH2
} bsp_pwm_channel_t;

void bsp_pwm_init(uint32_t frequency_hz, uint16_t ch1_duty, uint16_t ch2_duty);
void bsp_pwm_set_frequency(uint32_t frequency_hz);
void bsp_pwm_set_duty(bsp_pwm_channel_t channel, uint16_t duty);

#endif /* BSP_PWM_H */
