#include "resume.hpp"
#include <driver/gptimer.h>
#include <rmt_mdu_encoder.h>

namespace out::track::mdu {

/// TODO
esp_err_t init_encoder(mdu_encoder_config_t const& encoder_config) {
  assert(!encoder);
  return rmt_new_mdu_encoder(&encoder_config, &encoder);
}

namespace {

/// TODO
esp_err_t init_alarm() {
  ESP_ERROR_CHECK(gptimer_enable(gptimer));
  return gptimer_start(gptimer);
}

/// TODO
esp_err_t init_gpio(gpio_isr_t gpio_isr_handler) {
  ESP_ERROR_CHECK(gpio_isr_handler_add(ack_gpio_num, gpio_isr_handler, NULL));
  return gpio_set_level(enable_gpio_num, 1u);
}

}  // namespace

/// TODO
esp_err_t resume(mdu_encoder_config_t const& encoder_config,
                 gpio_isr_t gpio_isr_handler) {
  ESP_ERROR_CHECK(init_encoder(encoder_config));
  ESP_ERROR_CHECK(init_alarm());
  return init_gpio(gpio_isr_handler);
}

}  // namespace out::track::mdu