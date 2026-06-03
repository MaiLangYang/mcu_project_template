#include "bsp_motor.h"

#include "bsp_pwm.h"
#include "gd32a7xx.h"
#include "systick.h"

#define BSP_MOTOR_SD_GPIO_CLK   RCU_GPIOK
#define BSP_MOTOR_SD_GPIO_PORT  GPIOK
#define BSP_MOTOR_SD_PIN        GPIO_PIN_2

static bsp_motor_direction_t motor_direction = BSP_MOTOR_DIR_STOP;
static uint16_t motor_duty = 0U;

static uint16_t motor_clip_duty(uint16_t duty)
{
    if(duty > BSP_MOTOR_DUTY_MAX) {
        duty = BSP_MOTOR_DUTY_MAX;
    }

    return duty;
}

static void motor_sd_init(void)
{
    rcu_periph_clock_enable(BSP_MOTOR_SD_GPIO_CLK);

    gpio_bit_reset(BSP_MOTOR_SD_GPIO_PORT, BSP_MOTOR_SD_PIN);
    gpio_mode_set(BSP_MOTOR_SD_GPIO_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, BSP_MOTOR_SD_PIN);
    gpio_output_options_set(BSP_MOTOR_SD_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_LEVEL_2, BSP_MOTOR_SD_PIN);
}

static void motor_sd_enable(void)
{
    gpio_bit_set(BSP_MOTOR_SD_GPIO_PORT, BSP_MOTOR_SD_PIN);
}

static void motor_sd_disable(void)
{
    gpio_bit_reset(BSP_MOTOR_SD_GPIO_PORT, BSP_MOTOR_SD_PIN);
}

void bsp_motor_init(void)
{
    motor_sd_init();
    bsp_pwm_init(BSP_MOTOR_PWM_FREQ_HZ, 0U, 0U);
    bsp_motor_stop();
}

void bsp_motor_forward(uint16_t duty)
{
    duty = motor_clip_duty(duty);

    bsp_pwm_set_duty(BSP_PWM_CH2, 0U);
    bsp_pwm_set_duty(BSP_PWM_CH1, duty);

    if(duty > 0U) {
        motor_sd_enable();
        motor_direction = BSP_MOTOR_DIR_FORWARD;
    } else {
        motor_sd_disable();
        motor_direction = BSP_MOTOR_DIR_STOP;
    }

    motor_duty = duty;
}

void bsp_motor_reverse(uint16_t duty)
{
    duty = motor_clip_duty(duty);

    bsp_pwm_set_duty(BSP_PWM_CH1, 0U);
    bsp_pwm_set_duty(BSP_PWM_CH2, duty);

    if(duty > 0U) {
        motor_sd_enable();
        motor_direction = BSP_MOTOR_DIR_REVERSE;
    } else {
        motor_sd_disable();
        motor_direction = BSP_MOTOR_DIR_STOP;
    }

    motor_duty = duty;
}

void bsp_motor_stop(void)
{
    motor_sd_disable();
    bsp_pwm_set_duty(BSP_PWM_CH1, 0U);
    bsp_pwm_set_duty(BSP_PWM_CH2, 0U);
    motor_direction = BSP_MOTOR_DIR_STOP;
    motor_duty = 0U;
}

void bsp_motor_protect_reverse(uint16_t reverse_duty, uint32_t reverse_time_ms)
{
    bsp_motor_direction_t previous_direction = motor_direction;

    bsp_motor_stop();
    delay_1ms(BSP_MOTOR_PROTECT_REVERSE_DELAY_MS);

    reverse_duty = motor_clip_duty(reverse_duty);
    if(reverse_time_ms > 0U) {
        if(BSP_MOTOR_DIR_FORWARD == previous_direction) {
            bsp_motor_reverse(reverse_duty);
        } else if(BSP_MOTOR_DIR_REVERSE == previous_direction) {
            bsp_motor_forward(reverse_duty);
        } else {
            return;
        }

        delay_1ms(reverse_time_ms);
        bsp_motor_stop();
    }
}

bsp_motor_direction_t bsp_motor_get_direction(void)
{
    return motor_direction;
}

uint16_t bsp_motor_get_duty(void)
{
    return motor_duty;
}
