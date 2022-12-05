#ifndef __EX2_INT_H
#define __EX2_INT_H

#include "main.h"

//Define GPIO_PG14 mapping to our LED and Button
#define BUTTON_PIN              (0)  // Button PA0
#define LED_PIN                 (14) // LED GREEN PG14

extern void RunApp(void);

volatile uint8_t Led_on;

#endif /*__EX2_INT_H */