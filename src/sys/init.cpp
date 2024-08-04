/// Initialize sys service
///
/// \file   sys/init.cpp
/// \author Vincent Hamp
/// \date   13/08/2023

#include "init.hpp"
#include <memory>
#include "http/sta/service.hpp"
#include "log.h"
#include "service.hpp"

namespace sys {

namespace {

std::shared_ptr<Service> service;

}  // namespace

/// TODO
esp_err_t init() {
  if (http::sta::service) {
    service = std::make_shared<Service>();
    http::sta::service->subscribe(
      {.uri = "/sys/", .method = HTTP_GET}, service, &Service::getRequest);
  }
  return ESP_OK;
}

}  // namespace sys