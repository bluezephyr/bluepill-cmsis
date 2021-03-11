#include <stdbool.h>
#include "stm32f1xx.h"

extern uint32_t SystemCoreClock;

// References
// [1]: RM0008 Reference Manual cd00171190-stm32f101xx-stm32f102xx-stm32f103xx-stm32f105xx-and-stm32f107xx-advanced-armbased-32bit-mcus-stmicroelectronics.pdf

// Delay loop for 8 MHz CPU clock with optimizer enabled
void delay(uint32_t msec)
{
    for (uint32_t i = 0; i < 2000UL * msec; i++)
    {
        __NOP();
    }
}

void GPIO_Init(void)
{
    // 7.3.7 APB2 peripheral clock enable register (RCC_APB2ENR) in [1]
    // Enable APB2 peripheral clock for PORTC
    SET_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPCEN);

    // 9.2.2 Port configuration register high (GPIOx_CRH) (x=A..G) in [1]
    // Set Pin 13
    // CNF1=0, CNF0=0: General Purpose output Push-Pull
    // MODE1=0, MODE0=1: Maximum output speed 10 MHz
    MODIFY_REG(GPIOC->CRH, GPIO_CRH_CNF13_0 | GPIO_CRH_CNF13_1 | GPIO_CRH_MODE13_1, GPIO_CRH_MODE13_0);
}

int main(void)
{
    GPIO_Init();
    WRITE_REG(GPIOC->BSRR, GPIO_BSRR_BS13);

    while(true)
    {
        WRITE_REG(GPIOC->BSRR, GPIO_BSRR_BR13);
        delay(500);
        WRITE_REG(GPIOC->BSRR, GPIO_BSRR_BS13);
        delay(500);
    }
}
