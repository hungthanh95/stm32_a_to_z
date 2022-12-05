#include "ex2_int.h"

static void Init_EXTI(uint8_t pin);
static void Init_GPIO(void);

void RunApp(void) {
    uint8_t buttonPA0 = (uint8_t) BUTTON_PIN;

    Init_GPIO();
    Init_EXTI(buttonPA0);

    // Enable the NVIC interrupt for EXTI0
    NVIC_SetPriority(EXTI0_IRQn, 0x02);
    NVIC_EnableIRQ(EXTI0_IRQn);

    while(1) {
        if (Led_on) {
            GPIOG->ODR |= (1 << LED_PIN);
        } else {
            GPIOG->ODR &= ~(1 << LED_PIN);
        }
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

static void Init_EXTI(uint8_t pin) {
    // Enable 'APB2' clock
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    // Assign an EXTI line to GPIO bank
    // Set EXTI line to button PA0 (EXTI0 = 0x0000)
    // reset registers
    SYSCFG->EXTICR[ (pin / 4) ] &= ~(0xF << (pin % 4) * 4);
    SYSCFG->EXTICR[ (pin / 4) ] |= (0x0 << (pin % 4) * 4);

    // Setup the button's EXTI line as an interrupt
    EXTI->IMR |= (1 << pin);
    // Disable the 'rising edge' trigger (button release)
    EXTI->RTSR &= ~(1 << pin);
    //Enable the 'falling edge' trigger (button pressed)
    EXTI->FTSR |= (1 << pin);
}

void EXTI0_IRQHandler(void) {
    if (EXTI->PR & ( 1 << BUTTON_PIN)) {
        // clear the EXTI status flag
        EXTI->PR |= (1 << BUTTON_PIN);
        //toggle the global LED variable
        Led_on = !Led_on;
    }
}