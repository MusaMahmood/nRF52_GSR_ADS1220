/* Copyright (c) 2019 Musa Mahmood
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef AD5242_H__
#define AD5242_H__

#include "nrf_drv_twi.h"

#define AD5242_ADDRESS_READ (0x58 >> 1) //0b0101|1001
#define AD5242_ADDRESS_WRITE (0x58 >> 1) //0b0101|1000

#define AD5242_INSTRUCTION_RDAC1 0x00 //0b0|0|0|00|000

#define AD5242_SCL_PIN 18
#define AD5242_SDA_PIN 17

void ad5242_twi_init(nrf_drv_twi_t m_twi);

void ad5242_twi_uninit(nrf_drv_twi_t m_twi);

void ad5242_read_rdac1_value(nrf_drv_twi_t m_twi);

void ad5242_write_rdac1_value(nrf_drv_twi_t m_twi, uint8_t value);

void ad5242_write_rdac1_value_temp(nrf_drv_twi_t m_twi);


#endif // AD5242_H__