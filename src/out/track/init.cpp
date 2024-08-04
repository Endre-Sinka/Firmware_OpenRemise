/// Initialize MDU and DCC
///
/// \file   out/track/init.cpp
/// \author Vincent Hamp
/// \date   09/02/2023

#include <driver/gpio.h>
#include <driver/rmt_tx.h>
#include <array>
#include <cstring>
#include <limits>
#include "current_limit.hpp"
#include "dcc/task_function.hpp"
#include "mdu/task_function.hpp"

namespace out::track {

namespace {

/// TODO
esp_err_t init_gpio() {
  // Input
  {
    static constexpr gpio_config_t io_conf{
      .pin_bit_mask = 1ull << nfault_gpio_num,
      .mode = GPIO_MODE_INPUT,
      .pull_up_en = GPIO_PULLUP_DISABLE,
      .pull_down_en = GPIO_PULLDOWN_DISABLE,
      .intr_type = GPIO_INTR_DISABLE};
    ESP_ERROR_CHECK(gpio_config(&io_conf));
  }

  // Input
  {
    static constexpr gpio_config_t io_conf{.pin_bit_mask = 1ull << ack_gpio_num,
                                           .mode = GPIO_MODE_INPUT,
                                           .pull_up_en = GPIO_PULLUP_DISABLE,
                                           .pull_down_en =
                                             GPIO_PULLDOWN_DISABLE,
                                           .intr_type = GPIO_INTR_NEGEDGE};
    ESP_ERROR_CHECK(gpio_config(&io_conf));
    ESP_ERROR_CHECK(gpio_install_isr_service(ESP_INTR_FLAG_LEVEL2));
  }

  // Output
  {
    static constexpr gpio_config_t io_conf{
      .pin_bit_mask = 1ull << isel0_gpio_num | 1ull << isel1_gpio_num |
                      1ull << nsleep_gpio_num | 1ull << force_low_gpio_num |
                      1ull << enable_gpio_num | 1ull << dcc::bidi_en_gpio_num,
      .mode = GPIO_MODE_OUTPUT,
      .pull_up_en = GPIO_PULLUP_DISABLE,
      .pull_down_en = GPIO_PULLDOWN_DISABLE,
      .intr_type = GPIO_INTR_DISABLE};
    ESP_ERROR_CHECK(gpio_config(&io_conf));
    ESP_ERROR_CHECK(gpio_set_level(enable_gpio_num, 0u));
    ESP_ERROR_CHECK(gpio_set_level(dcc::bidi_en_gpio_num, 0u));

    // TODO remove? I'd like NSLEEP to always be 3.3V... but currently afraid
    // that there might be a fault I can't reset then
    return gpio_set_level(out::track::nsleep_gpio_num, 1u);
  }
}

/// TODO
esp_err_t init_channel() {
  static constexpr rmt_tx_channel_config_t chan_config{
    .gpio_num = in_gpio_num,
    .clk_src = RMT_CLK_SRC_DEFAULT,
    .resolution_hz = 1'000'000u,
    .mem_block_symbols =
      SOC_RMT_CHANNELS_PER_GROUP *
      SOC_RMT_MEM_WORDS_PER_CHANNEL,  // 8 channels sharing 384x32 bit RAM
    .trans_queue_depth = trans_queue_depth,
    .intr_priority = 3,
    .flags = {
      .invert_out = false,
      .with_dma = false,
      .io_loop_back = false,
      .io_od_mode = false,
    }};
  ESP_ERROR_CHECK(rmt_new_tx_channel(&chan_config, &channel));
  return rmt_enable(channel);
}

}  // namespace

/// TODO
esp_err_t init(BaseType_t xCoreID) {
  rx_queue.handle = xQueueCreate(rx_queue.size, sizeof(RxQueue::value_type));

  ESP_ERROR_CHECK(init_gpio());
  ESP_ERROR_CHECK(init_channel());

  //
  ESP_ERROR_CHECK(set_current_limit(CurrentLimit::_500mA));
  if (get_current_limit() != CurrentLimit::_500mA) assert(false);

  if (!xTaskCreatePinnedToCore(dcc::task_function,
                               dcc::task.name,
                               dcc::task.stack_depth,
                               NULL,
                               dcc::task.priority,
                               &dcc::task.handle,
                               xCoreID))
    assert(false);
  if (!xTaskCreatePinnedToCore(mdu::task_function,
                               mdu::task.name,
                               mdu::task.stack_depth,
                               NULL,
                               mdu::task.priority,
                               &mdu::task.handle,
                               xCoreID))
    assert(false);

  return ESP_OK;
}

}  // namespace out::track
