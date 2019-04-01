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

#ifndef ADS1220_H__
#define ADS1220_H__

// Pin definitions:
#define ADS1220_CS_PIN 11
#define ADS1220_SCLK_PIN 12
#define ADS1220_MOSI_PIN 13 // DIN on ADS1220 
#define ADS1220_MISO_PIN 14 // DOUT on ADS1220
#define ADS1220_DRDY_PIN 15

// Register addresses
#define ADS1220_REGADDR_CFG0 0x00
#define ADS1220_REGADDR_CFG1 0x01
#define ADS1220_REGADDR_CFG2 0x02
#define ADS1220_REGADDR_CFG3 0x03

// Default Values: 
#define ADS1220_REGDEFAULT_CFG0 0x00 // 0b0000|000|0 :[AINP = AIN0, AINN = AIN1, Gain=1, PGA enabled]
#define ADS1220_REGDEFAULT_CFG1 0x04 // 0b000|00|1|0|0 :[20 SPS, Normal mode, Continuous conversion, no temp sens, no burn-out sources]
#define ADS1220_REGDEFAULT_CFG2 0x00 // 0b00|00|0|000 :[Internal Reference, No FIR filter, switch between AIN3/REFN1 and AVSS open, IDAC off]
#define ADS1220_REGDEFAULT_CFG3 0x00 // 0b000|000|0|0 :[IDAC1 disabled, IDAC2 disabled, DRDY mode on DRDY only, Reserved (0)]

// Function Definitions



#endif // ADS1220_H__