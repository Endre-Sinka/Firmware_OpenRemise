///
///
/// \file   ota/init.cpp
/// \author Vincent Hamp
/// \date   13/06/2023

#include "init.hpp"
#include <esp_ota_ops.h>
#include <esp_partition.h>
#include <memory>
#include "http/sta/service.hpp"
#include "log.h"
#include "service.hpp"

namespace ota {

namespace {

std::shared_ptr<Service> service;

}  // namespace

/// TODO
esp_err_t init(BaseType_t xCoreID) {
  esp_partition_t const* boot_partition{esp_ota_get_boot_partition()};
  esp_partition_t const* running_partition{esp_ota_get_running_partition()};
  if (boot_partition != running_partition)
    LOGW("Configured OTA boot and running partition differs. This can happen "
         "if either the OTA boot data or preferred boot image become corrupted "
         "somehow.");

  if (http::sta::service) {
    service = std::make_shared<Service>(xCoreID);
    http::sta::service->subscribe({.uri = "/ota/"}, service, &Service::socket);
  }

  return ESP_OK;
}

}  // namespace ota