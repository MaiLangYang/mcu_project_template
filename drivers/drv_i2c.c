/*!
    \file    drv_i2c.c
    \brief   board I2C driver
*/

#include "drv_i2c.h"
#include <stddef.h>

static drv_i2c_status_t drv_i2c_error_check(void)
{
    if(SET == i2c_flag_get(BOARD_I2C, I2C_FLAG_NACK)) {
        i2c_flag_clear(BOARD_I2C, I2C_FLAG_NACK);
        i2c_stop_on_bus(BOARD_I2C);
        return DRV_I2C_ERR_NACK;
    }

    if((SET == i2c_flag_get(BOARD_I2C, I2C_FLAG_BERR)) ||
       (SET == i2c_flag_get(BOARD_I2C, I2C_FLAG_LOSTARB)) ||
       (SET == i2c_flag_get(BOARD_I2C, I2C_FLAG_TIMEOUT))) {
        i2c_flag_clear(BOARD_I2C, I2C_FLAG_BERR);
        i2c_flag_clear(BOARD_I2C, I2C_FLAG_LOSTARB);
        i2c_flag_clear(BOARD_I2C, I2C_FLAG_TIMEOUT);
        i2c_stop_on_bus(BOARD_I2C);
        return DRV_I2C_ERR_BUS;
    }

    return DRV_I2C_OK;
}

static drv_i2c_status_t drv_i2c_wait_flag(uint32_t flag, FlagStatus status)
{
    uint32_t timeout = BOARD_I2C_TIMEOUT;
    drv_i2c_status_t err;

    while(status != i2c_flag_get(BOARD_I2C, flag)) {
        err = drv_i2c_error_check();
        if(DRV_I2C_OK != err) {
            return err;
        }

        if(0U == timeout--) {
            i2c_stop_on_bus(BOARD_I2C);
            return DRV_I2C_ERR_TIMEOUT;
        }
    }

    return DRV_I2C_OK;
}

void drv_i2c_init(void)
{
    rcu_periph_clock_enable(BOARD_I2C_GPIO_RCU);
    rcu_periph_clock_enable(BOARD_I2C_RCU);

    gpio_af_set(BOARD_I2C_SCL_PORT, BOARD_I2C_GPIO_AF, BOARD_I2C_SCL_PIN);
    gpio_af_set(BOARD_I2C_SDA_PORT, BOARD_I2C_GPIO_AF, BOARD_I2C_SDA_PIN);

    gpio_mode_set(BOARD_I2C_SCL_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, BOARD_I2C_SCL_PIN);
    gpio_output_options_set(BOARD_I2C_SCL_PORT, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, BOARD_I2C_SCL_PIN);
    gpio_mode_set(BOARD_I2C_SDA_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, BOARD_I2C_SDA_PIN);
    gpio_output_options_set(BOARD_I2C_SDA_PORT, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, BOARD_I2C_SDA_PIN);

    i2c_deinit(BOARD_I2C);
    i2c_timing_config(BOARD_I2C, BOARD_I2C_TIMING_PSC, BOARD_I2C_TIMING_SCL_DELAY, BOARD_I2C_TIMING_SDA_DELAY);
    i2c_master_clock_config(BOARD_I2C, BOARD_I2C_TIMING_SCL_HIGH, BOARD_I2C_TIMING_SCL_LOW);
    i2c_enable(BOARD_I2C);
}

void drv_i2c_bus_reset(void)
{
    GPIO_BC(BOARD_I2C_SCL_PORT) |= BOARD_I2C_SCL_PIN;
    GPIO_BC(BOARD_I2C_SDA_PORT) |= BOARD_I2C_SDA_PIN;

    gpio_mode_set(BOARD_I2C_SCL_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, BOARD_I2C_SCL_PIN);
    gpio_output_options_set(BOARD_I2C_SCL_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, BOARD_I2C_SCL_PIN);
    gpio_mode_set(BOARD_I2C_SDA_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, BOARD_I2C_SDA_PIN);
    gpio_output_options_set(BOARD_I2C_SDA_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, BOARD_I2C_SDA_PIN);

    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();

    GPIO_BOP(BOARD_I2C_SCL_PORT) |= BOARD_I2C_SCL_PIN;
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    GPIO_BOP(BOARD_I2C_SDA_PORT) |= BOARD_I2C_SDA_PIN;

    gpio_mode_set(BOARD_I2C_SCL_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, BOARD_I2C_SCL_PIN);
    gpio_output_options_set(BOARD_I2C_SCL_PORT, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, BOARD_I2C_SCL_PIN);
    gpio_mode_set(BOARD_I2C_SDA_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, BOARD_I2C_SDA_PIN);
    gpio_output_options_set(BOARD_I2C_SDA_PORT, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, BOARD_I2C_SDA_PIN);
}

drv_i2c_status_t drv_i2c_write(uint8_t address_7bit, const uint8_t *data, uint16_t length)
{
    uint16_t i;
    drv_i2c_status_t status;

    if((NULL == data) || (0U == length) || (length > 255U)) {
        return DRV_I2C_ERR_BUS;
    }

    status = drv_i2c_wait_flag(I2C_FLAG_I2CBSY, RESET);
    if(DRV_I2C_OK != status) {
        return status;
    }

    i2c_automatic_end_disable(BOARD_I2C);
    i2c_reload_disable(BOARD_I2C);
    i2c_master_addressing(BOARD_I2C, ((uint32_t)address_7bit << 1U), I2C_MASTER_TRANSMIT);
    i2c_transfer_byte_number_config(BOARD_I2C, length);
    i2c_start_on_bus(BOARD_I2C);

    status = drv_i2c_wait_flag(I2C_FLAG_TBE, SET);
    if(DRV_I2C_OK != status) {
        return status;
    }

    for(i = 0U; i < length; i++) {
        i2c_data_transmit(BOARD_I2C, data[i]);
        status = drv_i2c_wait_flag(I2C_FLAG_TI, SET);
        if(DRV_I2C_OK != status) {
            return status;
        }
    }

    status = drv_i2c_wait_flag(I2C_FLAG_TC, SET);
    if(DRV_I2C_OK != status) {
        return status;
    }
    i2c_stop_on_bus(BOARD_I2C);

    status = drv_i2c_wait_flag(I2C_FLAG_STPDET, SET);
    if(DRV_I2C_OK != status) {
        return status;
    }
    i2c_flag_clear(BOARD_I2C, I2C_FLAG_STPDET);

    return DRV_I2C_OK;
}

drv_i2c_status_t drv_i2c_read(uint8_t address_7bit, uint8_t *data, uint16_t length)
{
    uint16_t i;
    drv_i2c_status_t status;

    if((NULL == data) || (0U == length) || (length > 255U)) {
        return DRV_I2C_ERR_BUS;
    }

    status = drv_i2c_wait_flag(I2C_FLAG_I2CBSY, RESET);
    if(DRV_I2C_OK != status) {
        return status;
    }

    i2c_automatic_end_disable(BOARD_I2C);
    i2c_reload_disable(BOARD_I2C);
    i2c_master_addressing(BOARD_I2C, ((uint32_t)address_7bit << 1U), I2C_MASTER_RECEIVE);
    i2c_transfer_byte_number_config(BOARD_I2C, length);
    i2c_start_on_bus(BOARD_I2C);

    for(i = 0U; i < length; i++) {
        status = drv_i2c_wait_flag(I2C_FLAG_RBNE, SET);
        if(DRV_I2C_OK != status) {
            return status;
        }
        data[i] = (uint8_t)i2c_data_receive(BOARD_I2C);
    }

    status = drv_i2c_wait_flag(I2C_FLAG_TC, SET);
    if(DRV_I2C_OK != status) {
        return status;
    }
    i2c_stop_on_bus(BOARD_I2C);

    status = drv_i2c_wait_flag(I2C_FLAG_STPDET, SET);
    if(DRV_I2C_OK != status) {
        return status;
    }
    i2c_flag_clear(BOARD_I2C, I2C_FLAG_STPDET);

    return DRV_I2C_OK;
}
