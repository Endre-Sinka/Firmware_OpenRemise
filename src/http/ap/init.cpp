/// Initialize HTTP service for access point
///
/// \file   http/ap/init.cpp
/// \author Vincent Hamp
/// \date   01/03/2023

#include "init.hpp"
#include <memory>
#include "service.hpp"

namespace http::ap {

namespace {

std::unique_ptr<Service> service;

}  // namespace

///
esp_err_t init() {
  service.reset(new Service{wifi::ap_records_queue.handle});
  return ESP_OK;
}

}  // namespace http::ap