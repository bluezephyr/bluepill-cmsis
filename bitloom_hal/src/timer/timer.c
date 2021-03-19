/*
 * Implementation of the BitLoom timer module for ARM STM32F103C8 (Bluepill)
 *
 * Copyright (c) 2021. BlueZephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */

#include <stm32f1xx.h>
#include <stdbool.h>
#include <config/timer_config.h>

volatile Tick_t timerTicks;

/*
 * Timer 0 is used to schedule the tasks.
 *
 * An interrupt is generated every ms and the scheduler timer tick function is
 * called.
 */
void timer_init (void)
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

void timer_start (void)
{
    uint32_t returnCode;
    timerTicks = 0;
    returnCode = SysTick_Config(SystemCoreClock / 1000);
    if (returnCode != 0)
    {
        while(true){}
    }
}

void timer_stop (void)
{
}

/*
 * Interrupt routine to update the ticks.
 */
void SysTick_Handler(void)
{
    ++timerTicks;
}
