#include <stdbool.h>
#include "stm32f1xx.h"

#define LED_FLASH_PERIOD 500u

enum LedAction
{
    idle,
    turnOn,
    turnOff
};

volatile enum LedAction ledAction = idle;
volatile enum LedAction nextAction = turnOn;
volatile uint16_t timer = 0;
extern uint32_t SystemCoreClock;

// References
// [1]: RM0008 Reference Manual cd00171190-stm32f101xx-stm32f102xx-stm32f103xx-stm32f105xx-and-stm32f107xx-advanced-armbased-32bit-mcus-stmicroelectronics.pdf

void SysTick_Handler(void)
{
    if (timer++ % LED_FLASH_PERIOD == 0)
    {
        ledAction = nextAction;
    }
}

void init_clocks(void)
{
    // Step 1
    // Set FLASH access latency to two wait states (010)
    MODIFY_REG(FLASH->ACR, FLASH_ACR_LATENCY, FLASH_ACR_LATENCY_1);

    // Enable HSE and wait for the clock to stabilize
    SET_BIT(RCC->CR, RCC_CR_HSEON);
    while (!READ_BIT(RCC->CR, RCC_CR_HSERDY));

    // Step 2
    // AHB Prescaler = /1 -- Maximum allowed frequency is 36MHz
    // PPRE1: APB low-speed prescaler (APB1): HCLK divided by 2
    // PLLMUL: x9
    // PLLSRC: 1: HSE oscillator clock selected as PLL input clock
    WRITE_REG(RCC->CFGR, RCC_CFGR_PPRE1_DIV2 | RCC_CFGR_PLLMULL9 | RCC_CFGR_PLLSRC);

    // Enable PLL and wait for it to stabilize
    SET_BIT(RCC->CR, RCC_CR_PLLON);
    while (!READ_BIT(RCC->CR, RCC_CR_PLLRDY));

    // Step 3
    // Set System Clock Mux source to PLL
    MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, RCC_CFGR_SW_PLL);

    SystemCoreClockUpdate();
}

void init_systemTick(void)
{
    uint32_t returnCode;
    returnCode = SysTick_Config(SystemCoreClock / 1000);
    if (returnCode != 0)
    {
        while(true){}
    }
}

void init_gpio(void)
{
    // 7.3.7 APB2 peripheral clock enable register (RCC_APB2ENR) in [1]
    // Enable APB2 peripheral clock for PORTC
    SET_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPCEN);

    // 9.2.2 Port configuration register high (GPIOx_CRH) (x=A..G) in [1]
    // Set Pin 13
    // CNF1=0, CNF0=0: General Purpose output Push-Pull
    // MODE1=1, MODE0=1: Maximum output speed 50 MHz
    MODIFY_REG(GPIOC->CRH, GPIO_CRH_CNF13, GPIO_CRH_MODE13);

    // Active low - disable at start
    WRITE_REG(GPIOC->BSRR, GPIO_BSRR_BS13);
}

int main(void)
{
    init_clocks();
    init_gpio();
    init_systemTick();

    while(true)
    {
        switch (ledAction)
        {
            case idle:
                break;
            case turnOn:
                WRITE_REG(GPIOC->BSRR, GPIO_BSRR_BR13);
                ledAction = idle;
                nextAction = turnOff;
                break;
            case turnOff:
                WRITE_REG(GPIOC->BSRR, GPIO_BSRR_BS13);
                ledAction = idle;
                nextAction = turnOn;
                break;
        }
    }
}

