/*!
    \file    gd32a50x_it.h
    \brief   interrupt service routines interface
*/

#ifndef GD32A50X_IT_H
#define GD32A50X_IT_H

#include "gd32a50x.h"

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);
void USART0_IRQHandler(void);

#endif /* GD32A50X_IT_H */
