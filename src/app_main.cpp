/// ESP-IDF application entry point
///
/// \file   app_main.cpp
/// \author Vincent Hamp
/// \date   26/12/2022

#include "analog/init.hpp"
#include "dcc/init.hpp"
#include "http/init.hpp"
#include "mdu/init.hpp"
#include "mem/init.hpp"
#include "ota/init.hpp"
#include "out/init.hpp"
#include "settings/init.hpp"
#include "sys/init.hpp"
#include "trace.hpp"
#include "udp/init.hpp"
#include "usb/init.hpp"
#include "utility.hpp"
#include "wifi/init.hpp"
#include "zusi/init.hpp"

/// ESP-IDF application entry point
extern "C" void app_main() {
  // Most important ones
  ESP_ERROR_CHECK(invoke_on_core(0, trace::init));
  ESP_ERROR_CHECK(invoke_on_core(0, mem::init));
  ESP_ERROR_CHECK(invoke_on_core(1, analog::init, 1));
  ESP_ERROR_CHECK(invoke_on_core(1, out::init, 1));

  // Don't change initialization order
  ESP_ERROR_CHECK(invoke_on_core(0, wifi::init));
  ESP_ERROR_CHECK(invoke_on_core(0, http::init));

  // Don't care from here on
  ESP_ERROR_CHECK(invoke_on_core(1, dcc::init, 1));
  ESP_ERROR_CHECK(invoke_on_core(1, mdu::init, 1));
  ESP_ERROR_CHECK(invoke_on_core(1, ota::init, 1));
  ESP_ERROR_CHECK(invoke_on_core(0, settings::init));
  ESP_ERROR_CHECK(invoke_on_core(0, sys::init));
  ESP_ERROR_CHECK(invoke_on_core(0, udp::init, 0));
  ESP_ERROR_CHECK(invoke_on_core(1, usb::init, 1));
  ESP_ERROR_CHECK(invoke_on_core(1, zusi::init, 1));

  for (;;) vTaskDelay(pdMS_TO_TICKS(1000u));
}
