#pragma once

#include <esp_err.h>

namespace mdu {

esp_err_t init(BaseType_t xCoreID);

}  // namespace mdu