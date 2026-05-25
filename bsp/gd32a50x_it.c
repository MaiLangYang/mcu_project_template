/*!
    \file    gd32a50x_it.c
    \brief   interrupt service routines
*/

#include "gd32a50x_it.h"
#include "drv_usart.h"
#include "systick.h"

#define SRAM_ECC_ERROR_HANDLE(s)       do{}while(1)
#define FLASH_ECC_ERROR_HANDLE(s)      do{}while(1)

void NMI_Handler(void)
{
    if(SET == syscfg_interrupt_flag_get(SYSCFG_INT_FLAG_SRAMECCMERR)) {
        SRAM_ECC_ERROR_HANDLE("SRAM multi-bits non-correction ECC error\r\n");
    } else if(SET == syscfg_interrupt_flag_get(SYSCFG_INT_FLAG_SRAMECCSERR)) {
        SRAM_ECC_ERROR_HANDLE("SRAM single bit correction ECC error\r\n");
    } else if(SET == syscfg_interrupt_flag_get(SYSCFG_INT_FLAG_FLASHECCERR)) {
        FLASH_ECC_ERROR_HANDLE("FLASH ECC error\r\n");
    } else {
        while(1) {
        }
    }
}

void HardFault_Handler(void)
{
    while(1) {
    }
}

void MemManage_Handler(void)
{
    while(1) {
    }
}

void BusFault_Handler(void)
{
    while(1) {
    }
}

void UsageFault_Handler(void)
{
    while(1) {
    }
}

void SVC_Handler(void)
{
    while(1) {
    }
}

void DebugMon_Handler(void)
{
    while(1) {
    }
}

void PendSV_Handler(void)
{
    while(1) {
    }
}

void SysTick_Handler(void)
{
    delay_decrement();
}

void USART0_IRQHandler(void)
{
    drv_usart_irq_handler();
}
