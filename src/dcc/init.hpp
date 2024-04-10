#pragma once

#include <esp_err.h>

namespace dcc {

esp_err_t init(BaseType_t xCoreID);

}  // namespace dcc