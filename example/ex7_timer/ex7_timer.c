#include "ex7_timer.h"

static void SetSystemClockTo16MHz(void);
static void ConfigureTimer3(void);
static void Init_GPIO(void);

void RunApp(void) {

    /* Set system clock to 16 MHz using HSI */
    SetSystemClockTo16MHz();

    /* Init pin PG14 for blink led */
    Init_GPIO();

    /* Configure timer 3 */
    ConfigureTimer3();

    while (1)
    {
        /* Toggle led  */
        GPIOG->ODR ^= (1 << LED_PIN);
        delay(2000);
    }
}

/*******************************************************************************
  \details  Providing milli seconds Delay by running Timer
  \return   void
  \retval   none
*******************************************************************************/
void delay(uint32_t ms )
{
  uint32_t i;
  for( i = 0; i <= ms; i++ )
  {
    /* Clear the count */
    TIM3->CNT = 0;

    /* Wait UIF to be set */
    while((TIM3->SR & TIM_SR_UIF) == 0);    /* This will generate 1ms delay */

    /* Reset UIF */
    TIM3->SR &= ~TIM_SR_UIF;
  }
}


/******************************************************************************
  \details  This function enables the HSI clock as a system clock and generate
            the 16MHz. The Internal HSI Clock is 16MHz. So, we are not using PLL
            and not dividing, Multiplying. So, we will get the 16MHz as it is.
  \return   void
  \retval   none
*******************************************************************************/
static void SetSystemClockTo16MHz()
{
    //Enable HSI clock
    if ((RCC->CR & RCC_CR_HSIRDY) == 0)
    {
        // HSION =1
        RCC->CR |= RCC_CR_HSION;
        // wait HSION ON
        while ((RCC->CR & RCC_CR_HSIRDY) == 0);

    }

    // select AHB prescaler to 1
    RCC->CFGR |= RCC_CFGR_HPRE_DIV1;

    // APB1 prescaler to 1
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV1;

    // APB2 prescaler to 1
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;

    // Select the HSI as system clock source
    RCC->CFGR &= (uint32_t)((uint32_t) ~(RCC_CFGR_SW));
    RCC->CFGR |= RCC_CFGR_SW_HSI;

    // Configure flash prefetch, intruction cache, data cache
    FLASH->ACR = FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_LATENCY_3WS;

    // disable HSE clock
    RCC->CR &= ~RCC_CR_HSEON;

}


/*******************************************************************************
  \details  This function configures the timer 3 to generate the 1ms delay.
  \return   void
  \retval   none
*******************************************************************************/
static void ConfigureTimer3(void)
{
    // Enable the APB clock for TIM3
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

    // system clock = 16MHz
    // timer clock speed = fCK_PSC / (PSC + 1)
    // 1 = 16 / (PSC + 1) => PSC = 15
    TIM3->PSC = 15;

    // 1MHz / 1000 = 1KHz = 1ms
    // so this will generate the 1ms delay
    TIM3->ARR = 999;

    // enable TIM3 module
    TIM3->CR1 = (1 << 0);
}

/*******************************************************************************
  \details  This function configures enable port G for blink LED
  \return   void
  \retval   none
*******************************************************************************/
static void Init_GPIO() {
    //Enable RCC for GPIOG
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;

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