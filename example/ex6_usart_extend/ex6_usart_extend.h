#ifndef __EX5_USART_H
#define __EX5_USART_H

#include "main.h"
#include "ringbuf.h"

//Define GPIO_PG14 mapping to our LED and Button
#define BUTTON_PIN              (0)  // Button PA0
#define LED_PIN                 (14) // LED GREEN PG14

// Define UART
#define UART1_RX                (10) // PA10
#define UART1_TX                (9)  // PA9

#ifdef DEBUG
  #define LOG( msg... ) printf( msg );
#else
  #define LOG( msg... ) ;
#endif

extern void RunApp(void);

uint64_t SystemCoreClock = 0;
#endif /*__EX5_USART_H */