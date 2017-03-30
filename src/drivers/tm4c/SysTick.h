#ifndef __SYSTICK_H__
#define __SYSTICK_H__

#include <stdint.h>
#include <tm4c123gh6pm.h>

#define WAIT_62US   5000
#define WAIT_125US  10000
#define WAIT_250US  20000
#define WAIT_500US  40000
#define WAIT_1MS    80000

#define SysTick_Enable()  NVIC_ST_CTRL_R |= 0x5
#define SysTick_Disable() NVIC_ST_CTRL_R &= ~0x5
#define SysTick_EnableInt()   NVIC_ST_CTRL_R |= 0x2
#define SysTick_DisableInt()  NVIC_ST_CTRL_R &= ~0x2
#define SysTick_EnableAll()   NVIC_ST_CTRL_R |= 0x7
#define SysTick_DisableAll()  NVIC_ST_CTRL_R &= ~0x7
#define SysTick_ChangeReload(a)  NVIC_ST_RELOAD_R = a
#define SysTick_Clear()   NVIC_ST_CURRENT_R = 0
#define SysTick_Reload    NVIC_ST_RELOAD_R
#define SysTick_Current   NVIC_ST_CURRENT_R

void SysTick_Init(uint32_t period,uint8_t priority,void(*task)(void));
void SysTick_ChangeHandler(void(*task)(void));
void SysTick_Handler(void);

// Time delay using busy wait.
// The delay parameter is in units of the core clock. (units of 62.5 nsec for 16 MHz clock)
void SysTick_Wait(uint32_t delay);

#endif
