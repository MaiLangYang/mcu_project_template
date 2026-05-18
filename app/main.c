/*!
    \file    main.c
    \brief   GD32A7xx single-core project template entry
*/

#include "gd32a7xx.h"
#include "main.h"
#include "systick.h"

static void cache_enable(void);

int main(void)
{
    cache_enable();
    systick_config();

    while(1) {
        /* Add application code here. */
    }
}

static void cache_enable(void)
{
    SCB_EnableICache();
    SCB_EnableDCache();
}

void led_spark(void)
{
    /* User heartbeat hook called by SysTick_Handler every 1 ms. */
}
