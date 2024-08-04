/// Cover /sys/ endpoint
///
/// \file   sys/service.cpp
/// \author Vincent Hamp
/// \date   13/08/2023

#pragma once

#include "service.hpp"
#include <ArduinoJson.h>
#include <esp_app_desc.h>
#include <algorithm>
#include <magic_enum.hpp>
#include "analog/convert.hpp"
#include "log.h"
#include "utility.hpp"

namespace sys {

using namespace analog;

/// TODO
http::Response Service::getRequest(http::Request const& req) {
  //
  JsonDocument doc;

  doc["mode"] = magic_enum::enum_name(mode.load());

  auto const app_desc{esp_app_get_description()};
  doc["version"] = app_desc->version;
  doc["project_name"] = app_desc->project_name;
  doc["compile_time"] = app_desc->time;
  doc["compile_date"] = app_desc->date;
  doc["idf_version"] = app_desc->idf_ver;

  doc["mdns"] = wifi::mdns_str;
  doc["ip"] = wifi::ip_str;
  doc["mac"] = wifi::mac_str;

  doc["heap"] = esp_get_free_heap_size();
  doc["internal_heap"] = esp_get_free_internal_heap_size();

  if (VoltagesQueue::value_type voltages;
      xQueuePeek(voltages_queue.handle, &voltages, 0u))
    doc["voltage"] =
      measurement2mV(static_cast<VoltageMeasurement>(
                       std::accumulate(cbegin(voltages), cend(voltages), 0) /
                       size(voltages)))
        .value();

  if (CurrentsQueue::value_type currents;
      xQueuePeek(currents_queue.handle, &currents, 0u))
    doc["current"] =
      measurement2mA(static_cast<CurrentMeasurement>(
                       std::accumulate(cbegin(currents), cend(currents), 0) /
                       size(currents)))
        .value();

  if (TemperatureQueue::value_type temp;
      xQueuePeek(temperature_queue.handle, &temp, 0u))
    doc["temperature"] = temp;

  //
  std::string json;
  json.reserve(1024uz);
  serializeJson(doc, json);

  return json;
}

}  // namespace sys