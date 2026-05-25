/*!
    \file    drv_adc.h
    \brief   board ADC driver
*/

#ifndef DRV_ADC_H
#define DRV_ADC_H

#include "board_config.h"
#include <stdint.h>

void drv_adc_init(void);
uint16_t drv_adc_read_channel(uint8_t channel);
uint16_t drv_adc_read_board_channel(void);

#endif /* DRV_ADC_H */
