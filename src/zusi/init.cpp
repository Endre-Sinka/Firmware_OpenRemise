#include "init.hpp"
#include <memory>
#include "http/sta/service.hpp"
#include "service.hpp"

namespace zusi {

namespace {

std::shared_ptr<Service> service;

}  // namespace

/// \todo document
esp_err_t init(BaseType_t xCoreID) {
  if (http::sta::service) {
    service = std::make_shared<Service>(xCoreID);
    http::sta::service->subscribe({.uri = "/zusi/"}, service, &Service::socket);
  }
  return ESP_OK;
}

}  // namespace zusi