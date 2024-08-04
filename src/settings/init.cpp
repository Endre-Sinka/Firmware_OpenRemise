/// Initialize
///
/// \file   settings/init.cpp
/// \author Vincent Hamp
/// \date   15/05/2023

#include "init.hpp"
#include <memory>
#include "http/sta/service.hpp"
#include "log.h"
#include "service.hpp"

namespace settings {

namespace {

std::shared_ptr<Service> service;

}  // namespace

/// TODO
esp_err_t init() {
  if (http::sta::service) {
    service = std::make_shared<Service>();
    http::sta::service->subscribe(
      {.uri = "/settings/", .method = HTTP_GET}, service, &Service::getRequest);
    http::sta::service->subscribe({.uri = "/settings/", .method = HTTP_POST},
                                  service,
                                  &Service::postRequest);
  }
  return ESP_OK;
}

}  // namespace settings