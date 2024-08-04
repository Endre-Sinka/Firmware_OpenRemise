/// NVS "settings" namespace
///
/// \file   mem/nvs/settings.cpp
/// \author Vincent Hamp
/// \date   10/02/2023

#include "settings.hpp"
#include <dcc/dcc.hpp>

namespace mem::nvs {

/// TODO
std::string Settings::getStationmDNS() const { return getBlob("sta_mdns"); }

/// TODO
esp_err_t Settings::setStationmDNS(std::string_view str) {
  // TODO validate mdns
  return setBlob("sta_mdns", str);
}

/// TODO
std::string Settings::getStationSSID() const { return getBlob("sta_ssid"); }

/// TODO
esp_err_t Settings::setStationSSID(std::string_view str) {
  // TODO validate SSID
  return setBlob("sta_ssid", str);
}

/// TODO
std::string Settings::getStationPassword() const { return getBlob("sta_pass"); }

/// TODO
esp_err_t Settings::setStationPassword(std::string_view str) {
  // TODO validate password
  return setBlob("sta_pass", str);
}

/// TODO
uint8_t Settings::getHttpReceiveTimeout() const {
  return getU8("http_rx_timeout");
}

/// TODO
esp_err_t Settings::setHttpReceiveTimeout(uint8_t value) {
  return value >= 5u && value <= 60u ? setU8("http_rx_timeout", value)
                                     : ESP_ERR_INVALID_ARG;
}

/// TODO
uint8_t Settings::getHttpTransmitTimeout() const {
  return getU8("http_tx_timeout");
}

/// TODO
esp_err_t Settings::setHttpTransmitTimeout(uint8_t value) {
  return value >= 5u && value <= 60u ? setU8("http_tx_timeout", value)
                                     : ESP_ERR_INVALID_ARG;
}

/// TODO
uint8_t Settings::getUsbReceiveTimeout() const {
  return getU8("usb_rx_timeout");
}

/// TODO
esp_err_t Settings::setUsbReceiveTimeout(uint8_t value) {
  return value >= 1u && value <= 10u ? setU8("usb_rx_timeout", value)
                                     : ESP_ERR_INVALID_ARG;
}

/// TODO
uint8_t Settings::getCurrentLimit() const { return getU8("current_limit"); }

/// TODO
esp_err_t Settings::setCurrentLimit(uint8_t value) {
  return value <= std::to_underlying(out::track::CurrentLimit::_4100mA)
           ? setU8("current_limit", value)
           : ESP_ERR_INVALID_ARG;
}

/// TODO
uint8_t Settings::getDccPreamble() const { return getU8("dcc_preamble"); }

/// TODO
esp_err_t Settings::setDccPreamble(uint8_t value) {
  return value >= DCC_TX_MIN_PREAMBLE_BITS && value <= DCC_TX_MAX_PREAMBLE_BITS
           ? setU8("dcc_preamble", value)
           : ESP_ERR_INVALID_ARG;
}

/// TODO
uint8_t Settings::getDccBit1Duration() const { return getU8("dcc_bit1_dur"); }

/// TODO
esp_err_t Settings::setDccBit1Duration(uint8_t value) {
  return value >= dcc::tx::Bit1Min && value <= dcc::tx::Bit1Max
           ? setU8("dcc_bit1_dur", value)
           : ESP_ERR_INVALID_ARG;
}

/// TODO
uint8_t Settings::getDccBit0Duration() const { return getU8("dcc_bit0_dur"); }

/// TODO
esp_err_t Settings::setDccBit0Duration(uint8_t value) {
  return value >= dcc::tx::Bit0Min && value <= dcc::tx::Bit0Max
           ? setU8("dcc_bit0_dur", value)
           : ESP_ERR_INVALID_ARG;
}

/// TODO
uint8_t Settings::getDccBiDiBitDuration() const {
  return getU8("dcc_bidibit_dur");
}

/// TODO
esp_err_t Settings::setDccBiDiBitDuration(uint8_t value) {
  return !value || (value >= 57u && value <= 61u)
           ? setU8("dcc_bidibit_dur", value)
           : ESP_ERR_INVALID_ARG;
}

}  // namespace mem::nvs