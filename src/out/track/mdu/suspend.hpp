/// Deinitialize peripherals when suspending MDU task
///
/// \file   out/track/mdu/suspend.hpp
/// \author Vincent Hamp
/// \date   10/04/2024

#pragma once

#include <esp_err.h>

namespace out::track::mdu {

esp_err_t deinit_encoder();
esp_err_t suspend();

}  // namespace out::track::mdu