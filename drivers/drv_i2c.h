/*!
    \file    drv_i2c.h
    \brief   board I2C driver
*/

#ifndef DRV_I2C_H
#define DRV_I2C_H

#include "board_config.h"
#include <stdint.h>

typedef enum {
    DRV_I2C_OK = 0,
    DRV_I2C_ERR_TIMEOUT,
    DRV_I2C_ERR_NACK,
    DRV_I2C_ERR_BUS
} drv_i2c_status_t;

void drv_i2c_init(void);
void drv_i2c_bus_reset(void);
drv_i2c_status_t drv_i2c_write(uint8_t address_7bit, const uint8_t *data, uint16_t length);
drv_i2c_status_t drv_i2c_read(uint8_t address_7bit, uint8_t *data, uint16_t length);

#endif /* DRV_I2C_H */
