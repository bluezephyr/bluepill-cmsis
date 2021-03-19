/*
 * Implementation of the I2C module for the Bluepill
 *
 * Copyright (c) 2021. BlueZephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */

#include "hal/i2c.h"
#include <stm32f1xx.h>

/*
 * I2C master mode - Section 26.3.3 [RM0008]
 *
 * Master mode is selected as soon as the Start condition is generated on the bus with a
 * START bit. The following is the required sequence in master mode.
 * - Program the peripheral input clock in I2C_CR2 register in order to generate correct timings
 * - Configure the clock control registers (I2C_CCR)
 * - Configure the rise time register (TRISE)
 * - Program the I2C_CR1 register to enable the peripheral
 * - Set the START bit in the I2C_CR1 register to generate a Start condition
 * Once the Start condition is sent:
 *   The SB bit is set by hardware and an interrupt is generated if the ITEVFEN bit is set.
 *
 * The peripheral input clock frequency must be at least:
 * - 2 MHz in Sm mode
 * - 4 MHz in Fm mode
 */
void i2c_init (void)
{
    // Port clock (APB2 peripheral) for the I2C pins must be enabled. Done in the port_config.

    // 8.3.8 APB1 peripheral clock enable register (RCC_APB1ENR)
    // RCC peripheral clock for I2C needs to be enabled first in order to be able
    // to modify other registers.
    SET_BIT(RCC->APB1ENR, RCC_APB1ENR_I2C1EN);

    // Set frequency in I2C_CR2 to match the APB frequency (max 50 MHz)
    // Use value 36 MHz to match APB1 domain (see timer.c)
    MODIFY_REG(I2C1->CR2, I2C_CR2_FREQ, 0x24);

    // I2C standard mode, 100 kHz
    // 26.6.8 I2C Clock control register (I2C_CCR)
    // Bits 11:0 CCR[11:0]: Clock control register in Fm/Sm mode (Master mode)
    // Controls the SCL clock in master mode.
    // Sm mode or SMBus: Thigh = CCR * TPCLK1, Tlow = CCR * TPCLK1
    // TPCLK1 for 36 MHz --> Period T = 28 ns
    // 100 kHZ (duty cycle 0.5) --> CCR * 28 --> 5000 = 180 (0xB4)
    MODIFY_REG(I2C1->CCR, (I2C_CCR_FS | I2C_CCR_DUTY | I2C_CCR_CCR), 0xB4);

    // Configure TRISE
    // 26.6.9 I2C TRISE register (I2C_TRISE)
    MODIFY_REG(I2C1->TRISE, I2C_TRISE_TRISE, 0x25);

    // Enable the peripheral
    // 26.6.1 I2C Control register 1 (I2C_CR1)
    SET_BIT(I2C1->CR1, I2C_CR1_PE);
}

i2c_result_t i2c_start (void)
{
    // Datasheet page 220
//    TWCR |= (1 << TWINT) | (1 << TWEN) | (1 << TWSTA);
//    i2c_wait_for_complete();
//
//    if (TW_STATUS != TW_START)
//    {
//        return i2c_operation_error;
//    }
//    return i2c_ok;
}

i2c_result_t i2c_restart (void)
{
//    TWCR |= (1 << TWINT) | (1 << TWEN) | (1 << TWSTA);
//    i2c_wait_for_complete();
//    if (TW_STATUS != TW_REP_START)
//    {
//        return i2c_operation_error;
//    }
//    return i2c_ok;
}

void i2c_stop (void)
{
//    TWCR |= (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
//    while (!(TWCR & (1 << TWSTO)));
}

i2c_result_t i2c_write_byte (uint8_t byte)
{
//    TWDR = byte;
//    TWCR = (1 << TWINT) | (1 << TWEN);
//    i2c_wait_for_complete();
//
//    switch (TW_STATUS)
//    {
//        case TW_MT_SLA_ACK:
//        case TW_MT_DATA_ACK:
//            return i2c_ack_received;
//        case TW_MT_SLA_NACK:
//        case TW_MT_DATA_NACK:
//            return i2c_nack_received;
//        case TW_MT_ARB_LOST:
//            return i2c_arbitration_lost;
//        default:
//            return i2c_operation_error;
//    }
}

uint8_t i2c_read_byte (uint8_t send_ack)
{
    // Not implemented
    //return i2c_operation_error;
}
