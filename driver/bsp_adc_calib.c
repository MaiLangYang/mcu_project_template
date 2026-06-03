#include "bsp_adc_calib.h"

#include "gd32a7xx.h"
#include <stdio.h>

#define BSP_ADC_CALIB_ADC                 ADC0
#define BSP_ADC_CALIB_ADC_CLK             RCU_ADC0
#define BSP_ADC_CALIB_ADC_GPIO_CLK        RCU_GPIOI
#define BSP_ADC_CALIB_ADC_GPIO_PORT       GPIOI
#define BSP_ADC_CALIB_ADC_PIN             GPIO_PIN_9
#define BSP_ADC_CALIB_ADC_CHANNEL         (0U)
#define BSP_ADC_CALIB_SAMPLE_TIME         (160U)
#define BSP_ADC_CALIB_MAX_RAW             (4095U)

#define BSP_ADC_CALIB_TRIG_TIMER          TIMER0
#define BSP_ADC_CALIB_TRIG_TIMER_CLK      RCU_TIMER0
#define BSP_ADC_CALIB_TRIG_GPIO_CLK       RCU_GPIOJ
#define BSP_ADC_CALIB_TRIG_GPIO_PORT      GPIOJ
#define BSP_ADC_CALIB_TRIG_PIN            GPIO_PIN_7
#define BSP_ADC_CALIB_TRIG_AF             GPIO_AF_6
#define BSP_ADC_CALIB_TIMER_CLOCK_HZ      (SystemCoreClock)

#define BSP_ADC_CALIB_TX_QUEUE_LEN        (128U)

typedef struct {
    uint32_t seq;
    uint16_t raw;
    uint16_t mv;
    uint16_t avg_mv;
} bsp_adc_calib_record_t;

static volatile bsp_adc_calib_record_t tx_queue[BSP_ADC_CALIB_TX_QUEUE_LEN];
static volatile uint16_t tx_head = 0U;
static volatile uint16_t tx_tail = 0U;
static volatile uint32_t sample_count = 0U;
static volatile uint32_t overflow_count = 0U;

static uint16_t avg_window[BSP_ADC_CALIB_AVG_WINDOW];
static uint32_t avg_sum = 0U;
static uint16_t avg_index = 0U;
static uint16_t avg_count = 0U;
static uint16_t decimation_count = 0U;

static uint16_t raw_to_mv(uint16_t raw)
{
    return (uint16_t)((((uint32_t)raw) * BSP_ADC_CALIB_VREF_MV + (BSP_ADC_CALIB_MAX_RAW / 2U)) / BSP_ADC_CALIB_MAX_RAW);
}

static void tx_queue_push(uint16_t raw, uint16_t avg_raw)
{
    uint16_t next_head = (uint16_t)((tx_head + 1U) % BSP_ADC_CALIB_TX_QUEUE_LEN);

    if(next_head == tx_tail) {
        tx_tail = (uint16_t)((tx_tail + 1U) % BSP_ADC_CALIB_TX_QUEUE_LEN);
        overflow_count++;
    }

    tx_queue[tx_head].seq = sample_count;
    tx_queue[tx_head].raw = raw;
    tx_queue[tx_head].mv = raw_to_mv(raw);
    tx_queue[tx_head].avg_mv = raw_to_mv(avg_raw);
    tx_head = next_head;
}

static uint16_t avg_update(uint16_t raw)
{
    uint32_t divisor;

    if(avg_count < BSP_ADC_CALIB_AVG_WINDOW) {
        avg_count++;
    } else {
        avg_sum -= avg_window[avg_index];
    }

    avg_window[avg_index] = raw;
    avg_sum += raw;
    avg_index = (uint16_t)((avg_index + 1U) % BSP_ADC_CALIB_AVG_WINDOW);

    divisor = avg_count;
    return (uint16_t)((avg_sum + (divisor / 2U)) / divisor);
}

static void adc_gpio_init(void)
{
    rcu_periph_clock_enable(BSP_ADC_CALIB_ADC_GPIO_CLK);

    gpio_mode_set(BSP_ADC_CALIB_ADC_GPIO_PORT, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, BSP_ADC_CALIB_ADC_PIN);
}

static void trigger_timer_gpio_init(void)
{
    rcu_periph_clock_enable(BSP_ADC_CALIB_TRIG_GPIO_CLK);

    gpio_af_set(BSP_ADC_CALIB_TRIG_GPIO_PORT, BSP_ADC_CALIB_TRIG_AF, BSP_ADC_CALIB_TRIG_PIN);
    gpio_mode_set(BSP_ADC_CALIB_TRIG_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, BSP_ADC_CALIB_TRIG_PIN);
    gpio_output_options_set(BSP_ADC_CALIB_TRIG_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_LEVEL_2, BSP_ADC_CALIB_TRIG_PIN);
}

static void trigger_timer_init(void)
{
    uint32_t period;
    timer_parameter_struct timer_initpara;
    timer_oc_parameter_struct timer_ocintpara;

    rcu_periph_clock_enable(BSP_ADC_CALIB_TRIG_TIMER_CLK);
    timer_deinit(BSP_ADC_CALIB_TRIG_TIMER);

    period = BSP_ADC_CALIB_TIMER_CLOCK_HZ / BSP_ADC_CALIB_SAMPLE_HZ;
    if(period < 2U) {
        period = 2U;
    }
    period -= 1U;

    timer_struct_para_init(&timer_initpara);
    timer_initpara.prescaler = 0U;
    timer_initpara.alignedmode = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection = TIMER_COUNTER_UP;
    timer_initpara.period = period;
    timer_initpara.clockdivision = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0U;
    timer_init(BSP_ADC_CALIB_TRIG_TIMER, &timer_initpara);

    timer_channel_output_struct_para_init(&timer_ocintpara);
    timer_ocintpara.outputstate = TIMER_CCX_ENABLE;
    timer_ocintpara.outputnstate = TIMER_CCXN_DISABLE;
    timer_ocintpara.ocpolarity = TIMER_OC_POLARITY_HIGH;
    timer_ocintpara.ocnpolarity = TIMER_OCN_POLARITY_HIGH;
    timer_ocintpara.ocidlestate = TIMER_OC_IDLE_STATE_LOW;
    timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;

    timer_channel_output_config(BSP_ADC_CALIB_TRIG_TIMER, TIMER_CH_0, &timer_ocintpara);
    timer_channel_output_mode_config(BSP_ADC_CALIB_TRIG_TIMER, TIMER_CH_0, TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(BSP_ADC_CALIB_TRIG_TIMER, TIMER_CH_0, TIMER_OC_SHADOW_ENABLE);
    timer_channel_output_pulse_value_config(BSP_ADC_CALIB_TRIG_TIMER, TIMER_CH_0, (period + 1U) / 2U);
    timer_auto_reload_shadow_enable(BSP_ADC_CALIB_TRIG_TIMER);
    timer_primary_output_config(BSP_ADC_CALIB_TRIG_TIMER, ENABLE);
}

static void adc0_init(void)
{
    rcu_periph_clock_enable(RCU_TRIGSEL);
    rcu_adc_clock_config(RCU_ADCSRC_HCLK, RCU_CKADC_DIV16);
    rcu_periph_clock_enable(BSP_ADC_CALIB_ADC_CLK);

    adc_deinit(BSP_ADC_CALIB_ADC);
    adc_mode_config(ADC_MODE_FREE);
    adc_resolution_config(BSP_ADC_CALIB_ADC, ADC_RESOLUTION_12B);
    adc_data_alignment_config(BSP_ADC_CALIB_ADC, ADC_DATAALIGN_RIGHT);
    adc_routine_sequence_conversion_mode_config(BSP_ADC_CALIB_ADC, ADC_ONE_SHOT_MODE);
    adc_channel_length_config(BSP_ADC_CALIB_ADC, ADC_ROUTINE_SEQUENCE, 1U);
    adc_sequence_channel_config(BSP_ADC_CALIB_ADC, ADC_ROUTINE_SEQUENCE, 0U, BSP_ADC_CALIB_ADC_CHANNEL, BSP_ADC_CALIB_SAMPLE_TIME);
    trigsel_init(TRIGSEL_OUTPUT_ADC0_ROUTRG, TRIGSEL_INPUT_TIMER0_CH0);
    adc_external_trigger_config(BSP_ADC_CALIB_ADC, ADC_ROUTINE_SEQUENCE, ADC_EXTERNAL_TRIGGER_RISING_EDGE);
    adc_interrupt_flag_clear(BSP_ADC_CALIB_ADC, ADC_INT_FLAG_EORC);
    adc_interrupt_enable(BSP_ADC_CALIB_ADC, ADC_INSERTED_SEQUENCE_NONE, ADC_INT_EORCIE);
    nvic_irq_enable(ADC0_IRQn, 2U, 0U);

    adc_calibration_mode_config(BSP_ADC_CALIB_ADC, ADC_CALIBRATION_OFFSET_MISMATCH);
    adc_calibration_number(BSP_ADC_CALIB_ADC, ADC_CALIBRATION_NUM15);
    (void)adc_calibration_enable(BSP_ADC_CALIB_ADC);
    adc_enable(BSP_ADC_CALIB_ADC);
}

void bsp_adc_calib_init(void)
{
    uint16_t i;

    for(i = 0U; i < BSP_ADC_CALIB_AVG_WINDOW; i++) {
        avg_window[i] = 0U;
    }

    tx_head = 0U;
    tx_tail = 0U;
    sample_count = 0U;
    overflow_count = 0U;
    avg_sum = 0U;
    avg_index = 0U;
    avg_count = 0U;
    decimation_count = 0U;

    adc_gpio_init();
    trigger_timer_gpio_init();
    trigger_timer_init();
    adc0_init();

    timer_enable(BSP_ADC_CALIB_TRIG_TIMER);
}

void bsp_adc_calib_irq_handler(void)
{
    uint16_t raw;
    uint16_t avg_raw;

    if(SET == adc_interrupt_flag_get(BSP_ADC_CALIB_ADC, ADC_INT_FLAG_EORC)) {
        raw = adc_sequence_data_read(BSP_ADC_CALIB_ADC, ADC_ROUTINE_SEQUENCE);
        adc_interrupt_flag_clear(BSP_ADC_CALIB_ADC, ADC_INT_FLAG_EORC);

        sample_count++;
        avg_raw = avg_update(raw);

        decimation_count++;
        if(decimation_count >= BSP_ADC_CALIB_UART_DECIMATION) {
            decimation_count = 0U;
            tx_queue_push(raw, avg_raw);
        }
    }
}

void bsp_adc_calib_poll(void)
{
    bsp_adc_calib_record_t record;

    while(tx_tail != tx_head) {
        __disable_irq();
        record = tx_queue[tx_tail];
        tx_tail = (uint16_t)((tx_tail + 1U) % BSP_ADC_CALIB_TX_QUEUE_LEN);
        __enable_irq();

        printf("%lu,%u,%u,%u\r\n",
               (unsigned long)record.seq,
               (unsigned int)record.raw,
               (unsigned int)record.mv,
               (unsigned int)record.avg_mv);
    }
}

uint32_t bsp_adc_calib_get_sample_count(void)
{
    return sample_count;
}

uint32_t bsp_adc_calib_get_overflow_count(void)
{
    return overflow_count;
}
