/*
 * Implementation of the PIN digital IO module for ARM STM32F103C8 (Bluepill)
 *
 * Copyright (c) 2021. BlueZephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */

#include <stdint.h>
#include <stm32f1xx.h>
#include "config/port_config.h"

// Note! The LED connected to pin PC13 is active low on the Bluepill board
void pin_digital_io_write_high(uint16_t pin_id)
{
    if (pin_id == PC13)
    {
        WRITE_REG(GPIOC->BSRR, GPIO_BSRR_BS13);
    }
}

void pin_digital_io_write_low(uint16_t pin_id)
{
    if (pin_id == PC13)
    {
        WRITE_REG(GPIOC->BSRR, GPIO_BSRR_BR13);
    }
}
