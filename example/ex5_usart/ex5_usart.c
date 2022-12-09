#include "ex5_usart.h"

static void Init_GPIO(void);
static void InitClock(void);
static void Init_GPIO_UART(void);
static void Init_USART(void);

void RunApp(void) {
    Init_GPIO();
    InitClock();
    Init_GPIO_UART();
    Init_USART();

    char rxb = '\0';
    while (1) {
        // wait for a byte if data to arrive
        while ( !(USART1->SR & USART_SR_RXNE) ) {};
        rxb = USART1->DR;
        // Re-transmit the received byte
        while ( !(USART1->SR & USART_SR_TXE) ) {};
        USART1->DR = rxb;
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

static void Init_GPIO_UART() {
    // Enable clock
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

    // reset MODER
    GPIOA->MODER &= ~( (0x3 << (UART1_RX  * 2) ) |
                       (0x3 << (UART1_TX * 2)) );
    // set MODER to alternative function mode
    GPIOA->MODER |= ( (0x2 << (UART1_RX  * 2) ) |
                      (0x2 << (UART1_TX * 2)) );

    //Set OTYPER  -> Push pull
    GPIOA->OTYPER &= ~( (0x1 << UART1_RX) |
                        (0x1 << UART1_TX) );

    // reset
    GPIOA->OSPEEDR &= ~( (0x3 << (UART1_RX * 2) ) |
                          (0x3 << (UART1_TX * 2) ) );

    // set OSPEEDR to high speed
    GPIOA->OSPEEDR |= ( (0x2 << (UART1_RX * 2) ) |
                        (0x2 << (UART1_TX * 2) ) );

    // set AFR (AF7 for USART1)
    GPIOA->AFR[ 1 ] &= ~( 0xF << ( (UART1_RX - 8) * 4 ) );
    GPIOA->AFR[ 1 ] |= ( 0x7 << ( (UART1_RX - 8) * 4 ) );

    GPIOA->AFR[ 1 ] &= ~( 0xF << ( (UART1_TX - 8) * 4 ) );
    GPIOA->AFR[ 1 ] |= ( 0x7 << ( (UART1_TX - 8) * 4 ) );
}

static void Init_USART() {
    // Disable UART
    USART1->CR1 &= ~(USART_CR1_UE);
    // config baurate register
    uint16_t uartdiv = SystemCoreClock / 9600;
    USART1->BRR = uartdiv;

    // Enable the USART
    USART1->CR1 |= (USART_CR1_RE | USART_CR1_TE | USART_CR1_UE);
}

static void InitClock(void) {
    RCC->CR |=  ( RCC_CR_HSION );
    while ( !( RCC->CR & RCC_CR_HSIRDY ) ) {};
    RCC->CFGR &= ~( RCC_CFGR_SW );
    RCC->CFGR |=  ( RCC_CFGR_SW_HSI );
    while ( ( RCC->CFGR & RCC_CFGR_SWS ) != RCC_CFGR_SWS_HSI ) {};
    SystemCoreClock = 16000000;
}
