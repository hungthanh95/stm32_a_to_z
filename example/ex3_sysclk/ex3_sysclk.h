#ifndef __EX3_SYSCLK_H
#define __EX3_SYSCLK_H

#include "main.h"
//Define PLL
#define RCC_PLLCFGR_PLLM_8                  (0x08UL << RCC_PLLCFGR_PLLM_Pos)
#define RCC_PLLCFGR_PLLN_24                 (0x018UL << RCC_PLLCFGR_PLLN_Pos)
//Define GPIO_PG14 mapping to our LED and Button
#define BUTTON_PIN              (0)  // Button PA0
#define LED_PIN                 (14) // LED GREEN PG14

extern void RunApp(void);

uint64_t Core_Clock_Hz = 48000000;
#endif /*__EX3_SYSCLK_H */