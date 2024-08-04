/// Initialize peripherals when resuming DCC task
///
/// \file   out/track/dcc/resume.cpp
/// \author Vincent Hamp
/// \date   10/02/2023

#include "resume.hpp"
#include <driver/gpio.h>
#include <driver/uart.h>
#include <algorithm>

namespace out::track::dcc {

/// TODO
esp_err_t init_encoder(dcc_encoder_config_t const& encoder_config) {
  assert(!encoder);
  return rmt_new_dcc_encoder(&encoder_config, &encoder);
}

/// TODO
esp_err_t init_rmt(rmt_tx_done_callback_t rmt_cb) {
  rmt_tx_event_callbacks_t cbs{.on_trans_done = rmt_cb};
  return rmt_tx_register_event_callbacks(channel, &cbs, NULL);
}

/// TODO
esp_err_t init_alarm(gptimer_alarm_cb_t gptimer_cb) {
  gptimer_event_callbacks_t cbs{.on_alarm = gptimer_cb};
  ESP_ERROR_CHECK(gptimer_register_event_callbacks(gptimer, &cbs, NULL));
  ESP_ERROR_CHECK(gptimer_enable(gptimer));
  return gptimer_start(gptimer);
}

/// TODO
esp_err_t init_bidi() {
  //
  static constexpr uart_config_t uart_config{
    .baud_rate = ::dcc::bidi::baudrate,
    .data_bits = UART_DATA_8_BITS,
    .parity = UART_PARITY_DISABLE,
    .stop_bits = UART_STOP_BITS_1,
    .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    .source_clk = UART_SCLK_DEFAULT,
  };
  ESP_ERROR_CHECK(uart_driver_install(
    UART_NUM_1, SOC_UART_FIFO_LEN * 2, 0, 0, NULL, ESP_INTR_FLAG_IRAM));
  ESP_ERROR_CHECK(uart_param_config(UART_NUM_1, &uart_config));
  return uart_set_pin(UART_NUM_1,
                      UART_PIN_NO_CHANGE,
                      bidi_rx_gpio_num,
                      UART_PIN_NO_CHANGE,
                      UART_PIN_NO_CHANGE);
}

/// TODO
esp_err_t init_gpio() { return gpio_set_level(enable_gpio_num, 1u); }

/// TODO
esp_err_t resume(dcc_encoder_config_t const& encoder_config,
                 rmt_tx_done_callback_t rmt_cb,
                 gptimer_alarm_cb_t gptimer_cb) {
  ESP_ERROR_CHECK(init_encoder(encoder_config));
  ESP_ERROR_CHECK(init_rmt(rmt_cb));
  ESP_ERROR_CHECK(init_alarm(gptimer_cb));
  ESP_ERROR_CHECK(init_bidi());
  return init_gpio();
}

}  // namespace out::track::dcc
