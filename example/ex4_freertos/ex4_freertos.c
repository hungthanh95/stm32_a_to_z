#include "ex4_freertos.h"

static void InitClock(void);
static void Init_GPIO(void);
static void Ledx_Task(void *arg);

void RunApp(void) {
    Init_GPIO();
    InitClock();
    /* Create LED task */
    xTaskCreate(Ledx_Task, "LED_Blink_1", 128, (void*)&Led3_st, configMAX_PRIORITIES-1, NULL);
    xTaskCreate(Ledx_Task, "LED_Blink_2", 128, (void*)&Led4_st, configMAX_PRIORITIES-1, NULL);

    // Start the scheduler.
    vTaskStartScheduler();
    while (1) {
        /* Do nothing */
    }
}

/* Init 2 LED PG13 and PG14 */
static void Init_GPIO() {
    //Enable RCC for GPIOG
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;

    //Init GPIOG, pin PG13, PG14 as output, push-pull
    //push-pull mean a pin either '1' or '0'
    //open-drain can only pull the pin to '0'
    //useful when we have multiple devices set to 'output' on the same wire
    // 00 -> reset pin
    GPIOG->MODER &= ~(0x3 << (LED_3 * 2));
    // 01 -> Output mode
    GPIOG->MODER |= (0x1 << (LED_3 * 2));
    //00 -> Push pull
    GPIOG->OTYPER &= ~(0x1 << LED_3);

    // 00 -> reset pin
    GPIOG->MODER &= ~(0x3 << (LED_4 * 2));
    // 01 -> Output mode
    GPIOG->MODER |= (0x1 << (LED_4 * 2));
    //00 -> Push pull
    GPIOG->OTYPER &= ~(0x1 << LED_4);
}

/* Init PLL clock with HSI 16MHz and system clock is 48MHz*/
static void InitClock(void) {
    // because we setup PLL with freq 32-48MHz clock speed
    // we need set 1 wait cycle for flash can read correct data.
    FLASH->ACR &= ~(0xF);
    FLASH->ACR |= (FLASH_ACR_LATENCY_1WS | FLASH_ACR_PRFTEN);

    // enable HSI oscillator
    RCC->CR |= RCC_CR_HSION;
    // while to it enable done
    while (!(RCC->CR & RCC_CR_HSIRDY)) {};

    // config the PLL use HSI with PLLDIV 8 and
    // PLLMUL 24
    RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLM |
                      RCC_PLLCFGR_PLLN |
                      RCC_PLLCFGR_PLLSRC);

    // set PLLM, PLLN, PLLSRC
    RCC->PLLCFGR |= (RCC_PLLCFGR_PLLM_8 |
                     RCC_PLLCFGR_PLLN_24 |
                     RCC_PLLCFGR_PLLSRC_HSI);

    // Enable PLL and wait for it to stable
    RCC->CR |= RCC_CR_PLLON;
    while (!(RCC->CR & RCC_CR_PLLRDY)) {}

    // select the PLL as the system clock source
    RCC->CFGR &= ~(RCC_CFGR_SW);
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    while (!(RCC->CFGR & RCC_CFGR_SWS_PLL)) {};
    //set the global clock speed
}

static void Ledx_Task(void *arg) {
    BlinkLedSt *blink = (BlinkLedSt*) arg;

    while (1)
    {
        // Toggle led
        GPIOG->ODR ^= (1 << blink->led);

        // Delay for a second-ish.
        vTaskDelay(pdMS_TO_TICKS(blink->delay));
    }
}
