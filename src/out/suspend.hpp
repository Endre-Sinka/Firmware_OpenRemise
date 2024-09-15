/// \todo document
///
/// \file   out/suspend.hpp
/// \author Vincent Hamp
/// \date   23/04/2023

#pragma once

#include <esp_err.h>

namespace out {

void reset_queue_and_message_buffers();
esp_err_t suspend();

}  // namespace out