#ifndef __EX2_INT_H
#define __EX2_INT_H


#include "main.h"
  #include "FreeRTOS.h"
  #include "task.h"

//Define PLL
#define RCC_PLLCFGR_PLLM_8                  (0x08UL << RCC_PLLCFGR_PLLM_Pos)
#define RCC_PLLCFGR_PLLN_24                 (0x018UL << RCC_PLLCFGR_PLLN_Pos)

//Define GPIO_PG14 mapping to our LED and Button
#define LED_3                   (13) // LED GREEN PG13
#define LED_4                   (14) // LED GREEN PG14

#define LED3_DELAY              (1000)
#define LED4_DELAY              (500)

extern void RunApp(void);

uint64_t Core_Clock_Hz = 48000000;

typedef struct
{
  uint8_t led;
  uint32_t delay;
} BlinkLedSt;

BlinkLedSt Led3_st = {
  (uint8_t)LED_3,
  (uint32_t)LED3_DELAY
};

BlinkLedSt Led4_st = {
  (uint8_t)LED_4,
  (uint32_t)LED4_DELAY
};


#endif /*__EX2_INT_H */