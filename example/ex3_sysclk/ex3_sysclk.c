#include "ex3_sysclk.h"

static void SetFlashWaitCycle(void);
static void InitClock(void);
static void EnableTim2(void);
static void StartTimer(TIM_TypeDef *TIMx, uint16_t ms);
static void StopTimer(TIM_TypeDef *TIMx);
static void Init_GPIO(void);

void RunApp(void) {
    SetFlashWaitCycle();
    Init_GPIO();
    InitClock();
    EnableTim2();
    StartTimer(TIM2, 1000);

    while (1) {

    }
}

void TIM2_IRQHandler(void) {
    if (TIM2->SR & TIM_SR_UIF) {
        TIM2->SR &= ~(TIM_SR_UIF);
        // toggle led output pin
        GPIOG->ODR ^= (1 << LED_PIN);
    }
}

static void Init_GPIO() {
    //Enable RCC for GPIOA, GPIOG
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;

    //Init GPIOA, pin PA0 as input mode, no pull-up or pull-down
    // 00 -> Input mode
    GPIOA->MODER &= ~(0x3 << (BUTTON_PIN  * 2));
    // Set PUPDR of PIN0 to 00
    GPIOA->PUPDR &= ~(0x3 << (BUTTON_PIN * 2));

    //Init GPIOG, pin PG14 as output, push-pull
    //push-pull mean a pin either '1' or '0'
    //open-drain can only pull the pin to '0'
    //useful when we have multiple devices set to 'output' on the same wire
    // 00 -> reset pin
    GPIOG->MODER &= ~(0x3 << (LED_PIN * 2));
    // 01 -> Output mode
    GPIOG->MODER |= (0x1 << (LED_PIN * 2));
    //00 -> Push pull
    GPIOG->OTYPER &= ~(0x1 << LED_PIN);
}

static void SetFlashWaitCycle(void) {
    // because we setup PLL with freq 32-48MHz clock speed
    // we need set 1 wait cycle for flash can read correct data.
    FLASH->ACR &= ~(0xF);
    FLASH->ACR |= (FLASH_ACR_LATENCY_1WS | FLASH_ACR_PRFTEN);
}

static void InitClock(void) {

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

static void EnableTim2(void) {
    // Enable TIM2
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    // Enable the NVIC interrupt for TIM2
    NVIC_SetPriority(TIM2_IRQn, 0x03);
    NVIC_EnableIRQ(TIM2_IRQn);
}

static void StartTimer(TIM_TypeDef *TIMx, uint16_t ms) {
    // Start by making sure the timer's counter is off
    TIMx->CR1 &= ~(TIM_CR1_CEN);
    // Next, reset the peripheral
    if (TIMx == TIM2) {
        RCC->APB1RSTR |= (RCC_APB1RSTR_TIM2RST);
        RCC->APB1RSTR &= ~(RCC_APB1RSTR_TIM2RST);
    }
    // Set the timer prescaler/autoreload timing registers.
    // (These are 16-bit timers, so this won't work with >65MHz.)
    TIMx->PSC = Core_Clock_Hz / 1000;
    TIMx->ARR =  ms;
    // send an update event to reset the timer and apply settings
    TIMx->EGR |= TIM_EGR_UG;
    // Enable the hardware interrupt
    TIMx->DIER |= TIM_DIER_UIE;
    // Enable the timer
    TIMx->CR1 |= TIM_CR1_CEN;
}

static void StopTimer(TIM_TypeDef *TIMx) {
    // Turn off the timer
    TIMx->CR1 &= ~(TIM_CR1_CEN);
    // Clear the pending update interrupt flag
    TIMx->SR &= ~(TIM_SR_UIF);
}