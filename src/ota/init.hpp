///
///
/// \file   ota/init.hpp
/// \author Vincent Hamp
/// \date   13/06/2023

#pragma once

#include <esp_err.h>

namespace ota {

esp_err_t init(BaseType_t xCoreID);

}  // namespace ota