/// TODO
///
/// \file   out/suspend.cpp
/// \author Vincent Hamp
/// \date   23/04/2023

#include "suspend.hpp"
#include "log.h"

namespace out {

/// TODO
void reset_queue_and_message_buffers() {
  xQueueReset(track::rx_queue.handle);
  while (!xMessageBufferReset(rx_message_buffer.handle)) {
    LOGW("Can't reset out::rx_message_buffer");
    vTaskDelay(pdMS_TO_TICKS(20u));
  }
  // Don't short circuit here!
  while (!xMessageBufferReset(tx_message_buffer.front_handle) |
         !xMessageBufferReset(tx_message_buffer.back_handle)) {
    LOGW("Can't reset out::tx_message_buffer");
    vTaskDelay(pdMS_TO_TICKS(20u));
  }
}

/// TODO
esp_err_t suspend() {
  reset_queue_and_message_buffers();
  mode.store(Mode::Suspended);
  return ESP_OK;
}

}  // namespace out