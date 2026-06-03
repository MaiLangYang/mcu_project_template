#ifndef BSP_KEY_H
#define BSP_KEY_H

#include "gd32a7xx.h"

typedef enum {
    BSP_KEY1 = 0,
    BSP_KEY2,
    BSP_KEY3,
    BSP_KEY4,
    BSP_KEY_COUNT
} bsp_key_t;

void bsp_key_init(void);
FlagStatus bsp_key_read(bsp_key_t key);

#endif /* BSP_KEY_H */
