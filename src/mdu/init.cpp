#include "init.hpp"
#include <memory>
#include "http/sta/service.hpp"
#include "service.hpp"

namespace mdu {

namespace {

std::shared_ptr<Service> service;

}  // namespace

///
esp_err_t init(BaseType_t xCoreID) {
  if (http::sta::service) {
    service = std::make_shared<Service>(xCoreID);
    http::sta::service->subscribe(
      {.uri = "/mdu/firmware/"}, service, &Service::firmwareSocket);
    http::sta::service->subscribe(
      {.uri = "/mdu/zpp/"}, service, &Service::zppSocket);
  }
  return ESP_OK;
}

}  // namespace mdu