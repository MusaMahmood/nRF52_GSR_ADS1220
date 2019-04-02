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

#include "nrf_drv_twi.h"
#include "nrf_log.h"
#include "app_util_platform.h"
#include "app_error.h"
#include "ad5242.h"

static const nrf_drv_twi_t m_twi = NRF_DRV_TWI_INSTANCE(1);

/* Indicates if operation on TWI has ended. */
static volatile bool m_xfer_done = false;
// AD5242 Value of RDAC:
static uint8_t m_rdac1_value;

__STATIC_INLINE void data_handler(uint8_t temp) {
  NRF_LOG_INFO("RDAC_VALUE: %d \r\n", temp); 
}

// TWI Handler:
void twi_handler(nrf_drv_twi_evt_t const *p_event, void *p_context) {
  switch (p_event->type) {
  case NRF_DRV_TWI_EVT_DONE:
    if (p_event->xfer_desc.type == NRF_DRV_TWI_XFER_RX) {
      data_handler(m_rdac1_value);
    }
    m_xfer_done = true;
    break;
  default:
    break;
  }
}

// TWI_init:
void ad5242_twi_init(void) {
  ret_code_t err_code;

  const nrf_drv_twi_config_t twi_config = {
      .scl = AD5242_SCL_PIN,
      .sda = AD5242_SDA_PIN,
      .frequency = NRF_TWI_FREQ_400K,
      .interrupt_priority = APP_IRQ_PRIORITY_HIGH,
      .clear_bus_init = false};

  err_code = nrf_drv_twi_init(&m_twi, &twi_config, twi_handler, NULL);
  APP_ERROR_CHECK(err_code);

  nrf_drv_twi_enable(&m_twi);
}

// Read Data
void ad5242_read_rdac1_value() {
  m_xfer_done = false;
  /* Read 1 byte from the specified address */
  ret_code_t err_code = nrf_drv_twi_rx(&m_twi, AD5242_ADDRESS_READ, (uint8_t *)&m_rdac1_value, sizeof(m_rdac1_value));
  APP_ERROR_CHECK(err_code);
  NRF_LOG_INFO("RDAC_VALUE: %d \r\n", m_rdac1_value); 
}

// Write Data
void ad5242_write_rdac1_value(uint8_t value) {
  m_xfer_done = false;
  uint8_t write_data[2] = {AD5242_INSTRUCTION_RDAC1, 0x00};
  memcpy(&write_data[1], &value, 1);
  ret_code_t err_code = nrf_drv_twi_tx(&m_twi, AD5242_ADDRESS_WRITE, write_data, sizeof(write_data), false);
  APP_ERROR_CHECK(err_code);
//  m_rdac1_value = value;
}
