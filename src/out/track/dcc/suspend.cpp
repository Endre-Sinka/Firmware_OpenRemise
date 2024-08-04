/// Deinitialize peripherals when suspending DCC task
///
/// \file   out/track/dcc/suspend.cpp
/// \author Vincent Hamp
/// \date   10/02/2023

#include "suspend.hpp"
#include <driver/gpio.h>
#include <driver/uart.h>
#include "../../suspend.hpp"

namespace out::track::dcc {

/// TODO
esp_err_t deinit_gpio() { return gpio_set_level(enable_gpio_num, 0u); }

/// TODO
esp_err_t deinit_bidi() {
  ESP_ERROR_CHECK(gpio_set_level(bidi_en_gpio_num, 0u));
  return uart_driver_delete(UART_NUM_1);
}

/// TODO
esp_err_t deinit_alarm() {
  gptimer_stop(gptimer);
  ESP_ERROR_CHECK(gptimer_set_raw_count(gptimer, 0ull));
  ESP_ERROR_CHECK(gptimer_disable(gptimer));
  gptimer_event_callbacks_t cbs{};
  ESP_ERROR_CHECK(gptimer_register_event_callbacks(gptimer, &cbs, NULL));
  return gptimer_set_alarm_action(gptimer, NULL);
}

/// TODO
esp_err_t deinit_rmt() {
  rmt_tx_event_callbacks_t cbs{};
  return rmt_tx_register_event_callbacks(channel, &cbs, NULL);
}

/// TODO
esp_err_t deinit_encoder() {
  ESP_ERROR_CHECK(rmt_del_encoder(encoder));
  encoder = NULL;
  return ESP_OK;
}

/// TODO
esp_err_t suspend() {
  ESP_ERROR_CHECK(deinit_gpio());
  ESP_ERROR_CHECK(deinit_bidi());
  ESP_ERROR_CHECK(deinit_alarm());
  ESP_ERROR_CHECK(deinit_rmt());
  ESP_ERROR_CHECK(deinit_encoder());
  return out::suspend();
}

}  // namespace out::track::dcc