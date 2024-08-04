///
///
/// \file   z21/init.cpp
/// \author Vincent Hamp
/// \date   15/04/2024

#include "init.hpp"
#include "dcc/service.hpp"
#include "http/sta/service.hpp"
#include "service.hpp"

namespace z21 {

///
esp_err_t init(BaseType_t xCoreID) {
  if (http::sta::service) {
    service = std::make_shared<Service>(xCoreID);
    service->dcc(dcc::service);
    dcc::service->z21(service, service);
    http::sta::service->subscribe({.uri = "/z21/"}, service, &Service::socket);
  }
  return ESP_OK;
}

}  // namespace z21
