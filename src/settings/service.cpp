/// Cover /settings/ endpoint
///
/// \file   settings/service.cpp
/// \author Vincent Hamp
/// \date   15/05/2023

#pragma once

#include "service.hpp"
#include <ArduinoJson.h>
#include "log.h"
#include "mem/nvs/settings.hpp"
#include "utility.hpp"

namespace settings {

/// \todo document
http::Response Service::getRequest(http::Request const& req) {
  mem::nvs::Settings nvs;

  // Read password (and hide it)
  auto sta_pass{nvs.getStationPassword()};
  std::ranges::fill(sta_pass, '*');

  //
  JsonDocument doc;
  doc["sta_mdns"] = nvs.getStationmDNS();
  doc["sta_ssid"] = nvs.getStationSSID();
  doc["sta_pass"] = sta_pass;
  doc["http_rx_timeout"] = nvs.getHttpReceiveTimeout();
  doc["http_tx_timeout"] = nvs.getHttpTransmitTimeout();
  doc["usb_rx_timeout"] = nvs.getUsbReceiveTimeout();
  doc["current_limit"] = std::to_underlying(nvs.getCurrentLimit());
  doc["current_sc_time"] = nvs.getCurrentShortCircuitTime();
  doc["dcc_preamble"] = nvs.getDccPreamble();
  doc["dcc_bit1_dur"] = nvs.getDccBit1Duration();
  doc["dcc_bit0_dur"] = nvs.getDccBit0Duration();
  doc["dcc_bidibit_dur"] = nvs.getDccBiDiBitDuration();
  doc["dcc_prog_type"] = nvs.getDccProgrammingType();
  doc["dcc_strtp_rs_pc"] = nvs.getDccStartupResetPacketCount();
  doc["dcc_cntn_rs_pc"] = nvs.getDccContinueResetPacketCount();
  doc["dcc_prog_pc"] = nvs.getDccProgramPacketCount();
  doc["dcc_verify_bit1"] = nvs.getDccBitVerifyTo1();
  doc["dcc_ack_cur"] = nvs.getDccProgrammingAckCurrent();
  doc["dcc_flags"] = nvs.getDccFlags();
  doc["mdu_preamble"] = nvs.getMduPreamble();
  doc["mdu_ackreq"] = nvs.getMduAckreq();

  //
  std::string json;
  json.reserve(1024uz);
  serializeJson(doc, json);

  return json;
}

/// \todo document
http::Response Service::postRequest(http::Request const& req) {
  LOGI("%s", __PRETTY_FUNCTION__);
  LOGI("uri %s", req.uri.c_str());
  LOGI("body %s", req.body.c_str());

  // Validate body
  if (!validate_json(req.body))
    return std::unexpected<std::string>{"415 Unsupported Media Type"};

  // Deserialize (this only creates meta data which points to the source)
  JsonDocument doc;
  if (auto const err{deserializeJson(doc, req.body)}) {
    LOGE("Deserialization failed %s", err.c_str());
    return std::unexpected<std::string>{"500 Internal Server Error"};
  }

  mem::nvs::Settings nvs;

  if (JsonVariantConst v{doc["sta_mdns"]}; v.is<std::string>())
    if (auto const str{v.as<std::string>()}; nvs.setStationmDNS(str) != ESP_OK)
      return std::unexpected<std::string>{"422 Unprocessable Entity"};

  if (JsonVariantConst v{doc["sta_ssid"]}; v.is<std::string>())
    if (auto const str{v.as<std::string>()}; nvs.setStationSSID(str) != ESP_OK)
      return std::unexpected<std::string>{"422 Unprocessable Entity"};

  if (JsonVariantConst v{doc["sta_pass"]}; v.is<std::string>())
    if (auto const str{v.as<std::string>()};
        nvs.setStationPassword(str) != ESP_OK)
      return std::unexpected<std::string>{"422 Unprocessable Entity"};

  if (JsonVariantConst v{doc["http_rx_timeout"]}; v.is<uint8_t>())
    if (nvs.setHttpReceiveTimeout(v.as<uint8_t>()) != ESP_OK)
      return std::unexpected<std::string>{"422 Unprocessable Entity"};

  if (JsonVariantConst v{doc["http_tx_timeout"]}; v.is<uint8_t>())
    if (nvs.setHttpTransmitTimeout(v.as<uint8_t>()) != ESP_OK)
      return std::unexpected<std::string>{"422 Unprocessable Entity"};

  if (JsonVariantConst v{doc["usb_rx_timeout"]}; v.is<uint8_t>())
    if (nvs.setUsbReceiveTimeout(v.as<uint8_t>()) != ESP_OK)
      return std::unexpected<std::string>{"422 Unprocessable Entity"};

  if (JsonVariantConst v{doc["current_limit"]}; v.is<uint8_t>())
    if (nvs.setCurrentLimit(
          static_cast<out::track::CurrentLimit>(v.as<uint8_t>())) != ESP_OK)
      return std::unexpected<std::string>{"422 Unprocessable Entity"};

  if (JsonVariantConst v{doc["current_sc_time"]}; v.is<uint8_t>())
    if (nvs.setCurrentShortCircuitTime(v.as<uint8_t>()) != ESP_OK)
      return std::unexpected<std::string>{"422 Unprocessable Entity"};

  if (JsonVariantConst v{doc["dcc_preamble"]}; v.is<uint8_t>())
    if (nvs.setDccPreamble(v.as<uint8_t>()) != ESP_OK)
      return std::unexpected<std::string>{"422 Unprocessable Entity"};

  if (JsonVariantConst v{doc["dcc_bit1_dur"]}; v.is<uint8_t>())
    if (nvs.setDccBit1Duration(v.as<uint8_t>()) != ESP_OK)
      return std::unexpected<std::string>{"422 Unprocessable Entity"};

  if (JsonVariantConst v{doc["dcc_bit0_dur"]}; v.is<uint8_t>())
    if (nvs.setDccBit0Duration(v.as<uint8_t>()) != ESP_OK)
      return std::unexpected<std::string>{"422 Unprocessable Entity"};

  if (JsonVariantConst v{doc["dcc_bidibit_dur"]}; v.is<uint8_t>())
    if (nvs.setDccBiDiBitDuration(v.as<uint8_t>()) != ESP_OK)
      return std::unexpected<std::string>{"422 Unprocessable Entity"};

  if (JsonVariantConst v{doc["dcc_prog_type"]}; v.is<uint8_t>())
    if (nvs.setDccProgrammingType(v.as<uint8_t>()) != ESP_OK)
      return std::unexpected<std::string>{"422 Unprocessable Entity"};

  if (JsonVariantConst v{doc["dcc_strtp_rs_pc"]}; v.is<uint8_t>())
    if (nvs.setDccStartupResetPacketCount(v.as<uint8_t>()) != ESP_OK)
      return std::unexpected<std::string>{"422 Unprocessable Entity"};

  if (JsonVariantConst v{doc["dcc_cntn_rs_pc"]}; v.is<uint8_t>())
    if (nvs.setDccContinueResetPacketCount(v.as<uint8_t>()) != ESP_OK)
      return std::unexpected<std::string>{"422 Unprocessable Entity"};

  if (JsonVariantConst v{doc["dcc_prog_pc"]}; v.is<uint8_t>())
    if (nvs.setDccProgramPacketCount(v.as<uint8_t>()) != ESP_OK)
      return std::unexpected<std::string>{"422 Unprocessable Entity"};

  if (JsonVariantConst v{doc["dcc_verify_bit1"]}; v.is<bool>())
    if (nvs.setDccBitVerifyTo1(v.as<bool>()) != ESP_OK)
      return std::unexpected<std::string>{"422 Unprocessable Entity"};

  if (JsonVariantConst v{doc["dcc_ack_cur"]}; v.is<uint8_t>())
    if (nvs.setDccProgrammingAckCurrent(v.as<uint8_t>()) != ESP_OK)
      return std::unexpected<std::string>{"422 Unprocessable Entity"};

  if (JsonVariantConst v{doc["dcc_flags"]}; v.is<uint8_t>())
    if (nvs.setDccFlags(v.as<uint8_t>()) != ESP_OK)
      return std::unexpected<std::string>{"422 Unprocessable Entity"};

  if (JsonVariantConst v{doc["mdu_preamble"]}; v.is<uint8_t>())
    if (nvs.setMduPreamble(v.as<uint8_t>()) != ESP_OK)
      return std::unexpected<std::string>{"422 Unprocessable Entity"};

  if (JsonVariantConst v{doc["mdu_ackreq"]}; v.is<uint8_t>())
    if (nvs.setMduAckreq(v.as<uint8_t>()) != ESP_OK)
      return std::unexpected<std::string>{"422 Unprocessable Entity"};

  return {};
}

}  // namespace settings