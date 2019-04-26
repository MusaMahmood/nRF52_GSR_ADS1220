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

#include "tmp116.h"
#include "app_error.h"
#include "app_util_platform.h"
#include "nrf_delay.h"
#include "nrf_drv_twi.h"
#include "nrf_log.h"

/* Indicates if operation on TWI has ended. */
static volatile bool m_xfer_done = false;

void twi_handler_tmp(nrf_drv_twi_evt_t const *p_event, void *p_context) {
  switch (p_event->type) {
  case NRF_DRV_TWI_EVT_DONE:
    // if (p_event->xfer_desc.type == NRF_DRV_TWI_XFER_RX) {
    //    data_handler(m_sample);
    // }
//    NRF_LOG_INFO("[TWI_1] - TWI-EVT-DONE\r\n");
    m_xfer_done = true;
    break;
  case NRF_DRV_TWI_EVT_ADDRESS_NACK:
    NRF_LOG_INFO("[TWI_1] - ADDRESS NACK \r\n");
    break;
  case NRF_DRV_TWI_EVT_DATA_NACK:
    NRF_LOG_INFO("[TWI_1] - DATA NACK \r\n");
    break;
  default:
    break;
  }
}

void tmp116_twi_init(nrf_drv_twi_t m_twi) {
  ret_code_t err_code;

  const nrf_drv_twi_config_t twi_config = {
      .scl = TMP116_SCL_PIN,
      .sda = TMP116_SDA_PIN,
      .frequency = NRF_TWI_FREQ_100K,
      .interrupt_priority = APP_IRQ_PRIORITY_HIGH,
      .clear_bus_init = true};

  err_code = nrf_drv_twi_init(&m_twi, &twi_config, twi_handler_tmp, NULL);
  APP_ERROR_CHECK(err_code);

  nrf_drv_twi_enable(&m_twi);
  NRF_LOG_INFO("[TWI_1] Initialized: TMP116 \r\n");
}

void tmp116_twi_uninit(nrf_drv_twi_t m_twi) {
  nrf_drv_twi_disable(&m_twi);
  nrf_drv_twi_uninit(&m_twi);
  NRF_LOG_INFO("[TWI_1] Uninitialized: TMP116 \r\n");
}

void tmp116_set_mode(nrf_drv_twi_t m_twi) {
  m_xfer_done = false;
  ret_code_t err_code;

  uint8_t reg[3] = {TMP116_REG_CONF, 0x02, 0x30};
  err_code = nrf_drv_twi_tx(&m_twi, TMP116_ADDRESS, reg, sizeof(reg), false);
  APP_ERROR_CHECK(err_code);
  while (m_xfer_done == false)
    ;

  reg[0] = TMP116_REG_TEMP;
  m_xfer_done = false;
  err_code = nrf_drv_twi_tx(&m_twi, TMP116_ADDRESS, reg, 1, false);
  APP_ERROR_CHECK(err_code);
  while (m_xfer_done == false)
    ;
}

uint16_t tmp116_read_data(nrf_drv_twi_t m_twi) {
  m_xfer_done = false;
  uint16_t m_sample;

  ret_code_t err_code = nrf_drv_twi_rx(&m_twi, TMP116_ADDRESS, (uint8_t *)&m_sample, sizeof(m_sample));
  APP_ERROR_CHECK(err_code);
  while (m_xfer_done == false)
    ;
  // Reinterpret and print:
  m_sample = ((m_sample << 8) & 0xff00) | ((m_sample >> 8) & 0x00ff);
//  NRF_LOG_INFO("Temperature: 0x%x (as int).\r\n", m_sample);
  return m_sample;
}