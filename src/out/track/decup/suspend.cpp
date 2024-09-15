/// Deinitialize peripherals when suspending DECUP task
///
/// \file   out/track/decup/suspend.cpp
/// \author Vincent Hamp
/// \date   14/08/2024

#include "suspend.hpp"
#include <driver/gpio.h>
#include "../../suspend.hpp"

namespace out::track::decup {

namespace {

/// \todo document
esp_err_t deinit_gpio() {
  ESP_ERROR_CHECK(gpio_set_level(enable_gpio_num, 0u));
  return gpio_isr_handler_remove(ack_gpio_num);
}

/// \todo document
esp_err_t deinit_alarm() {
  gptimer_stop(gptimer);
  ESP_ERROR_CHECK(gptimer_set_raw_count(gptimer, 0ull));
  ESP_ERROR_CHECK(gptimer_disable(gptimer));
  gptimer_event_callbacks_t cbs{};
  ESP_ERROR_CHECK(gptimer_register_event_callbacks(gptimer, &cbs, NULL));
  return gptimer_set_alarm_action(gptimer, NULL);
}

}  // namespace

/// \todo document
esp_err_t deinit_encoder() {
  ESP_ERROR_CHECK(rmt_del_encoder(encoder));
  encoder = NULL;
  return ESP_OK;
}

/// \todo document
esp_err_t suspend() {
  ESP_ERROR_CHECK(deinit_gpio());
  ESP_ERROR_CHECK(deinit_alarm());
  ESP_ERROR_CHECK(deinit_encoder());
  return out::suspend();
}

}  // namespace out::track::decup