#include "bsp_pwm.h"

#include "gd32a7xx.h"

#define BSP_PWM_TIMER             TIMER7
#define BSP_PWM_TIMER_CLK         RCU_TIMER7
#define BSP_PWM_CH1_GPIO_CLK      RCU_GPIOK
#define BSP_PWM_CH1_GPIO_PORT     GPIOK
#define BSP_PWM_CH1_PIN           GPIO_PIN_1
#define BSP_PWM_CH1_AF            GPIO_AF_13
#define BSP_PWM_CH2_GPIO_CLK      RCU_GPIOG
#define BSP_PWM_CH2_GPIO_PORT     GPIOG
#define BSP_PWM_CH2_PIN           GPIO_PIN_2
#define BSP_PWM_CH2_AF            GPIO_AF_13
#define BSP_PWM_TIMER_CLOCK_HZ    (SystemCoreClock)

static uint32_t pwm_period = 999U;
static uint16_t pwm_ch1_duty = 0U;
static uint16_t pwm_ch2_duty = 0U;

static uint32_t duty_to_pulse(uint16_t duty)
{
    uint32_t clipped_duty = duty;

    if(clipped_duty > BSP_PWM_DUTY_MAX) {
        clipped_duty = BSP_PWM_DUTY_MAX;
    }

    return (uint32_t)((((uint64_t)(pwm_period + 1U)) * clipped_duty) / BSP_PWM_DUTY_MAX);
}

static void pwm_gpio_init(void)
{
    rcu_periph_clock_enable(BSP_PWM_CH1_GPIO_CLK);
    rcu_periph_clock_enable(BSP_PWM_CH2_GPIO_CLK);

    gpio_af_set(BSP_PWM_CH1_GPIO_PORT, BSP_PWM_CH1_AF, BSP_PWM_CH1_PIN);
    gpio_mode_set(BSP_PWM_CH1_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, BSP_PWM_CH1_PIN);
    gpio_output_options_set(BSP_PWM_CH1_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_LEVEL_2, BSP_PWM_CH1_PIN);

    gpio_af_set(BSP_PWM_CH2_GPIO_PORT, BSP_PWM_CH2_AF, BSP_PWM_CH2_PIN);
    gpio_mode_set(BSP_PWM_CH2_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, BSP_PWM_CH2_PIN);
    gpio_output_options_set(BSP_PWM_CH2_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_LEVEL_2, BSP_PWM_CH2_PIN);
}

static void pwm_timer_base_config(uint32_t frequency_hz)
{
    uint32_t timer_clock;
    uint32_t prescaler;
    timer_parameter_struct timer_initpara;

    if(0U == frequency_hz) {
        frequency_hz = BSP_PWM_DEFAULT_FREQ_HZ;
    }

    timer_clock = BSP_PWM_TIMER_CLOCK_HZ;
    prescaler = (uint32_t)(((uint64_t)timer_clock) / (((uint64_t)frequency_hz) * 65536ULL));
    if(prescaler > 0xFFFFU) {
        prescaler = 0xFFFFU;
    }

    pwm_period = (timer_clock / (prescaler + 1U)) / frequency_hz;
    if(0U == pwm_period) {
        pwm_period = 1U;
    }
    pwm_period -= 1U;
    if(pwm_period > 0xFFFFU) {
        pwm_period = 0xFFFFU;
    }

    timer_struct_para_init(&timer_initpara);
    timer_initpara.prescaler = (uint16_t)prescaler;
    timer_initpara.alignedmode = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection = TIMER_COUNTER_UP;
    timer_initpara.period = pwm_period;
    timer_initpara.clockdivision = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0U;
    timer_init(BSP_PWM_TIMER, &timer_initpara);
}

static void pwm_channel_config(uint16_t ch1_duty, uint16_t ch2_duty)
{
    timer_oc_parameter_struct timer_ocintpara;

    timer_channel_output_struct_para_init(&timer_ocintpara);
    timer_ocintpara.outputstate = TIMER_CCX_ENABLE;
    timer_ocintpara.outputnstate = TIMER_CCXN_DISABLE;
    timer_ocintpara.ocpolarity = TIMER_OC_POLARITY_HIGH;
    timer_ocintpara.ocnpolarity = TIMER_OCN_POLARITY_HIGH;
    timer_ocintpara.ocidlestate = TIMER_OC_IDLE_STATE_LOW;
    timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;

    timer_channel_output_config(BSP_PWM_TIMER, TIMER_CH_1, &timer_ocintpara);
    timer_channel_output_config(BSP_PWM_TIMER, TIMER_CH_2, &timer_ocintpara);
    timer_channel_output_mode_config(BSP_PWM_TIMER, TIMER_CH_1, TIMER_OC_MODE_PWM0);
    timer_channel_output_mode_config(BSP_PWM_TIMER, TIMER_CH_2, TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(BSP_PWM_TIMER, TIMER_CH_1, TIMER_OC_SHADOW_ENABLE);
    timer_channel_output_shadow_config(BSP_PWM_TIMER, TIMER_CH_2, TIMER_OC_SHADOW_ENABLE);

    bsp_pwm_set_duty(BSP_PWM_CH1, ch1_duty);
    bsp_pwm_set_duty(BSP_PWM_CH2, ch2_duty);
}

void bsp_pwm_init(uint32_t frequency_hz, uint16_t ch1_duty, uint16_t ch2_duty)
{
    pwm_ch1_duty = ch1_duty;
    pwm_ch2_duty = ch2_duty;

    pwm_gpio_init();
    rcu_periph_clock_enable(BSP_PWM_TIMER_CLK);
    timer_deinit(BSP_PWM_TIMER);
    pwm_timer_base_config(frequency_hz);
    pwm_channel_config(ch1_duty, ch2_duty);
    timer_auto_reload_shadow_enable(BSP_PWM_TIMER);
    timer_primary_output_config(BSP_PWM_TIMER, ENABLE);
    timer_enable(BSP_PWM_TIMER);
}

void bsp_pwm_set_frequency(uint32_t frequency_hz)
{
    timer_disable(BSP_PWM_TIMER);
    pwm_timer_base_config(frequency_hz);
    bsp_pwm_set_duty(BSP_PWM_CH1, pwm_ch1_duty);
    bsp_pwm_set_duty(BSP_PWM_CH2, pwm_ch2_duty);
    timer_enable(BSP_PWM_TIMER);
}

void bsp_pwm_set_duty(bsp_pwm_channel_t channel, uint16_t duty)
{
    uint32_t pulse;

    if(duty > BSP_PWM_DUTY_MAX) {
        duty = BSP_PWM_DUTY_MAX;
    }
    pulse = duty_to_pulse(duty);

    if(BSP_PWM_CH1 == channel) {
        pwm_ch1_duty = duty;
        timer_channel_output_pulse_value_config(BSP_PWM_TIMER, TIMER_CH_1, pulse);
    } else if(BSP_PWM_CH2 == channel) {
        pwm_ch2_duty = duty;
        timer_channel_output_pulse_value_config(BSP_PWM_TIMER, TIMER_CH_2, pulse);
    } else {
        /* invalid channel */
    }
}
