#include "ex1_gpio.h"

static void Init_GPIO();

void RunApp(void) {
    uint8_t button_pressed = 0;

    Init_GPIO();
    while (1)
    {
        uint32_t pa0_val = (~GPIOA->IDR) & (1 << BUTTON_PIN);
        if (pa0_val) {
            if (!button_pressed) {
                GPIOG->ODR ^= (1 << LED_PIN);
            }
            button_pressed = 1;
        } else {
            button_pressed = 0;
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