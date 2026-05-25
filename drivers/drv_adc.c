/*!
    \file    drv_adc.c
    \brief   board ADC driver
*/

#include "drv_adc.h"
#include "systick.h"

void drv_adc_init(void)
{
    rcu_periph_clock_enable(BOARD_ADC_GPIO_RCU);
    rcu_periph_clock_enable(BOARD_ADC_RCU);
    rcu_adc_clock_config(RCU_CKADC_CKAHB_DIV10);

    gpio_mode_set(BOARD_ADC_PORT, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, BOARD_ADC_PIN);

    adc_mode_config(ADC_MODE_FREE);
    adc_data_alignment_config(BOARD_ADC, ADC_DATAALIGN_RIGHT);
    adc_special_function_config(BOARD_ADC, ADC_CONTINUOUS_MODE, DISABLE);
    adc_special_function_config(BOARD_ADC, ADC_SCAN_MODE, DISABLE);
    adc_channel_length_config(BOARD_ADC, ADC_REGULAR_CHANNEL, 1U);
    adc_regular_channel_config(BOARD_ADC, 0U, BOARD_ADC_CHANNEL, BOARD_ADC_SAMPLE_TIME);
    adc_external_trigger_source_config(BOARD_ADC, ADC_REGULAR_CHANNEL, ADC0_1_EXTTRIG_REGULAR_NONE);
    adc_external_trigger_config(BOARD_ADC, ADC_REGULAR_CHANNEL, ENABLE);

    adc_enable(BOARD_ADC);
    delay_1ms(1U);
    adc_calibration_enable(BOARD_ADC);
}

uint16_t drv_adc_read_channel(uint8_t channel)
{
    adc_regular_channel_config(BOARD_ADC, 0U, channel, BOARD_ADC_SAMPLE_TIME);
    adc_software_trigger_enable(BOARD_ADC, ADC_REGULAR_CHANNEL);

    while(!adc_flag_get(BOARD_ADC, ADC_FLAG_EOC)) {
    }
    adc_flag_clear(BOARD_ADC, ADC_FLAG_EOC);

    return (uint16_t)adc_regular_data_read(BOARD_ADC);
}

uint16_t drv_adc_read_board_channel(void)
{
    return drv_adc_read_channel(BOARD_ADC_CHANNEL);
}
