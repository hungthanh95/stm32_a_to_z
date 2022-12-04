#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>
#include "stm32f429xx.h"

// Define your example you want to run
#define EXAMPLE_1

#ifdef EXAMPLE_1
#include "ex1_gpio.h"
#endif

#ifdef __cplusplus
}
#endif

#endif /*__MAIN_H */