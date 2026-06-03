#ifndef BSP_OLED_PORT_H
#define BSP_OLED_PORT_H

#include "gd32a7xx.h"

void bsp_oled_port_init(void);
void bsp_oled_scl_high(void);
void bsp_oled_scl_low(void);
void bsp_oled_sda_high(void);
void bsp_oled_sda_low(void);
FlagStatus bsp_oled_sda_read(void);

#endif /* BSP_OLED_PORT_H */
