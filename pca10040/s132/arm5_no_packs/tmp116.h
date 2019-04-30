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

#ifndef TMP116_H__
#define TMP116_H__

#include "nrf.h"
#include "nrf_drv_twi.h"

#define TMP116_ADDRESS (0x90 >> 1)

#define TMP116_REG_TEMP 0x00
#define TMP116_REG_CONF 0x01

#define TMP116_SCL_PIN 19
#define TMP116_SDA_PIN 20

void tmp116_twi_init(nrf_drv_twi_t m_twi);

void tmp116_twi_uninit(nrf_drv_twi_t m_twi);

void tmp116_set_mode(nrf_drv_twi_t m_twi);

uint16_t tmp116_read_data(nrf_drv_twi_t m_twi);

#endif