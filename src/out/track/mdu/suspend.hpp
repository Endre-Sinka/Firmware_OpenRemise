#pragma once

#include <esp_err.h>

namespace out::track::mdu {

esp_err_t deinit_encoder();
esp_err_t suspend();

}  // namespace out::track::mdu