/// Initialize HTTP service for station
///
/// \file   http/sta/init.cpp
/// \author Vincent Hamp
/// \date   01/03/2023

#include "init.hpp"
#include <memory>
#include <span>
#include "service.hpp"

namespace http::sta {

///
esp_err_t init() {
  service = std::make_shared<Service>();
  return ESP_OK;
}

}  // namespace http::sta