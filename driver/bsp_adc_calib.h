#ifndef BSP_ADC_CALIB_H
#define BSP_ADC_CALIB_H

#include <stdint.h>

#define BSP_ADC_CALIB_SAMPLE_HZ          (3000U)
#define BSP_ADC_CALIB_UART_DECIMATION    (10U)
#define BSP_ADC_CALIB_AVG_WINDOW         (32U)
#define BSP_ADC_CALIB_VREF_MV            (3300U)

void bsp_adc_calib_init(void);
void bsp_adc_calib_poll(void);
void bsp_adc_calib_irq_handler(void);
uint32_t bsp_adc_calib_get_sample_count(void);
uint32_t bsp_adc_calib_get_overflow_count(void);

#endif /* BSP_ADC_CALIB_H */
