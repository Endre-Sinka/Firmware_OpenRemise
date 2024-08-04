///
///
/// \file   z21/init.hpp
/// \author Vincent Hamp
/// \date   15/04/2024

#pragma once

#include <esp_err.h>

namespace z21 {

esp_err_t init(BaseType_t xCoreID);

}  // namespace z21