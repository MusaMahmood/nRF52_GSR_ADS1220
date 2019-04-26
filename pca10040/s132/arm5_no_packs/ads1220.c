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

#include "ads1220.h"
#include "app_error.h"
#include "app_util_platform.h"
#include "ble_sg.h"
#include "nrf_delay.h"
#include "nrf_drv_spi.h"
#include "nrf_gpio.h"
#include "nrf_log.h"
#include <stdio.h>

uint8_t ads1220_default_regs[] = {
    ADS1220_REGDEFAULT_CFG0,
    ADS1220_REGDEFAULT_CFG1,
    ADS1220_REGDEFAULT_CFG2,
    ADS1220_REGDEFAULT_CFG3,
};

static const nrf_drv_spi_t spi = NRF_DRV_SPI_INSTANCE(2);
static volatile bool spi_xfer_done;

/**
 * @brief SPI user event handler.
 * @param event
 */
void spi_event_handler(nrf_drv_spi_evt_t const *p_event,
    void *p_context) {
  spi_xfer_done = true;
}

void ads_spi_init(void) {
  nrf_drv_spi_config_t spi_config = NRF_DRV_SPI_DEFAULT_CONFIG;
  spi_config.bit_order = NRF_DRV_SPI_BIT_ORDER_MSB_FIRST;
  //SCLK = 1MHz is right speed because fCLK = (1/2)*SCLK, and fMOD = fCLK/4, and fMOD MUST BE 128kHz. Do the math.
  spi_config.frequency = NRF_DRV_SPI_FREQ_1M;
  spi_config.irq_priority = APP_IRQ_PRIORITY_HIGHEST; //APP_IRQ_PRIORITY_HIGHEST;
  spi_config.mode = NRF_DRV_SPI_MODE_1;               //CPOL = 0 (Active High); CPHA = TRAILING (1)
  spi_config.miso_pin = ADS1220_MISO_PIN;
  spi_config.sck_pin = ADS1220_SCLK_PIN;
  spi_config.mosi_pin = ADS1220_MOSI_PIN;
  spi_config.ss_pin = ADS1220_CS_PIN;
  spi_config.orc = 0x55;
  APP_ERROR_CHECK(nrf_drv_spi_init(&spi, &spi_config, spi_event_handler, NULL));
}

/* Control Functions */

void ads1220_init_default_regs(void) {
  uint8_t i = 0;
  uint8_t num_registers = 4;
  uint8_t txrx_size = num_registers + 1;
  uint8_t tx_data_spi[txrx_size]; //Size = 6 bytes
  uint8_t rx_data_spi[txrx_size]; //Size = 6 bytes

  for (i = 0; i < txrx_size; i++) {
    tx_data_spi[i] = 0; // Set array to zero.
    rx_data_spi[i] = 0; // Set array to zero.
  }

  tx_data_spi[0] = ADS1220_WREG_OPCODE;

  memcpy(&tx_data_spi[1], &ads1220_default_regs[0], num_registers);

  spi_xfer_done = false;
  APP_ERROR_CHECK(nrf_drv_spi_transfer(&spi, tx_data_spi, num_registers + 1, rx_data_spi, num_registers + 1));
  nrf_delay_ms(10);
  while (!spi_xfer_done) {
    __WFE();
  }
  NRF_LOG_INFO(" Power-on reset and initialization procedure..\r\n");
}

void ads1220_check_written_regs(void) {
  uint8_t i = 0;
  uint8_t num_registers = 4;
  uint8_t txrx_size = num_registers + 2;
  uint8_t tx_data_spi[txrx_size]; //Size = 6 bytes
  uint8_t rx_data_spi[txrx_size]; //Size = 6 bytes

  for (i = 0; i < txrx_size; i++) {
    tx_data_spi[i] = 0; // Set array to zero.
    rx_data_spi[i] = 0; // Set array to zero.
  }

  tx_data_spi[0] = ADS1220_RREG_OPCODE;
  spi_xfer_done = false;
  APP_ERROR_CHECK(nrf_drv_spi_transfer(&spi, tx_data_spi, 1, rx_data_spi, 4));
  while (!spi_xfer_done) {
    __WFE();
  }
  NRF_LOG_INFO("RX_DATA_SPI: \n");
  NRF_LOG_HEXDUMP_DEBUG(&rx_data_spi, 5);
}

void ads1220_reset(void) {
  uint8_t tx_data_spi;
  uint8_t rx_data_spi;

  tx_data_spi = ADS1220_RESET_OPCODE;

  spi_xfer_done = false;
  APP_ERROR_CHECK(nrf_drv_spi_transfer(&spi, &tx_data_spi, 1, &rx_data_spi, 1));
  while (!spi_xfer_done) {
    __WFE();
  }
  NRF_LOG_INFO(" Reset ADS1220..\r\n");
}

void ads1220_start_sync(void) {
  uint8_t tx_data_spi;
  uint8_t rx_data_spi;

  tx_data_spi = ADS1220_START_SYNC_OPCODE;

  spi_xfer_done = false;
  APP_ERROR_CHECK(nrf_drv_spi_transfer(&spi, &tx_data_spi, 1, &rx_data_spi, 1));
  while (!spi_xfer_done) {
    __WFE();
  }
  NRF_LOG_INFO(" Start/Sync ADS1220..\r\n");
}

void ads1220_powerdown(void) {
  uint8_t tx_data_spi;
  uint8_t rx_data_spi;

  tx_data_spi = ADS1220_PWDN_OPCODE;

  spi_xfer_done = false;
  APP_ERROR_CHECK(nrf_drv_spi_transfer(&spi, &tx_data_spi, 1, &rx_data_spi, 1));
  while (!spi_xfer_done) {
    __WFE();
  }
  NRF_LOG_INFO(" Powerdown ADS1220..\r\n");
}

void get_gsr_data(ble_sg_t *p_sg) {
  uint8_t rx_data[3];
  spi_xfer_done = false;
  APP_ERROR_CHECK(nrf_drv_spi_transfer(&spi, NULL, NULL, rx_data, 3));
  while (!spi_xfer_done) {
    __WFE();
  }
  memcpy_fast(&p_sg->sg_ch1_buffer[p_sg->sg_ch1_count], &rx_data[0], 3);
  p_sg->sg_ch1_count += 3;
}
