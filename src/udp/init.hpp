/// Initialize UDP
///
/// \file   udp/init.hpp
/// \author Vincent Hamp
/// \date   04/04/2024

#pragma once

#include <esp_err.h>

namespace udp {

esp_err_t init(BaseType_t xCoreID);

}  // namespace udp