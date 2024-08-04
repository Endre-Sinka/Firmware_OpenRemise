/// DCC_EIN protocol receive task function
///
/// \file   usb/dcc_ein/rx_task_function.cpp
/// \author Vincent Hamp
/// \date   10/02/2023

#include "rx_task_function.hpp"
#include <array>
#include <charconv>
#include <cstring>
#include <ulf/dcc_ein.hpp>
#include "../tx_task_function.hpp"
#include "log.h"
#include "utility.hpp"

namespace usb::dcc_ein {

/// Receive DCC packet from senddcc string
///
/// \return dcc::Packet dcc::Packet created from senddcc string
/// \return std::nullopt on timeout
std::optional<dcc::Packet> receive_dcc_packet() {
  std::array<char, rx_stream_buffer.size> stack;
  size_t count{};

  for (;;) {
    // Receive single character
    auto const bytes_received{
      xStreamBufferReceive(rx_stream_buffer.handle,
                           &stack[count],
                           1uz,
                           pdMS_TO_TICKS(rx_task.timeout))};
    count += bytes_received;
    if (!bytes_received || count >= size(stack)) return std::nullopt;

    // Convert senddcc string to DCC packet
    auto const packet{
      ulf::dcc_ein::senddcc_str2packet(std::string_view{cbegin(stack), count})};
    // Not enough characters
    if (!packet) count = 0uz;
    // Complete packet
    else if (*packet) return *packet;
  }
}

namespace {

/// Send DCC packet to out::tx_message_buffer front
///
/// \param  packet  DCC packet
void send_to_front(dcc::Packet const& packet) {
  xMessageBufferSend(out::tx_message_buffer.front_handle,
                     data(packet),
                     size(packet),
                     portMAX_DELAY);
}

/// Send DCC packet to out::tx_message_buffer back
///
/// \param  packet  DCC packet
void send_to_back(dcc::Packet const& packet) {
  xMessageBufferSend(out::tx_message_buffer.back_handle,
                     data(packet),
                     size(packet),
                     portMAX_DELAY);
}

/// Send DCC idle packets to out::tx_message_buffer back
void send_idle_packets_to_back() {
  static constexpr auto idle_packet{dcc::make_idle_packet()};
  while (xMessageBufferSpacesAvailable(out::tx_message_buffer.back_handle) >
         out::tx_message_buffer.size * 0.5)
    send_to_back(idle_packet);
}

/// TODO
void ack_senddcc_str() {
  auto const space_used{
    out::tx_message_buffer.size -
    xMessageBufferSpacesAvailable(out::tx_message_buffer.front_handle)};
  auto const str{::ulf::dcc_ein::ack2senddcc_str('b', space_used)};
  xStreamBufferSend(tx_stream_buffer.handle,
                    data(str),
                    size(str),
                    pdMS_TO_TICKS(tx_task.timeout));
}

/// Actual usb::dcc_ein::rx_task loop
void loop() {
  auto const timeout{get_usb_receive_timeout()};
  TickType_t then{xTaskGetTickCount() + pdMS_TO_TICKS(timeout)};

  for (;;) {
    send_idle_packets_to_back();

    // Return on timeout
    if (auto const now{xTaskGetTickCount()}; now >= then) return;
    // In case we got a packet, reset timeout
    else if (auto const packet{receive_dcc_packet()}) {
      then = now + pdMS_TO_TICKS(timeout);
      send_to_front(*packet);
      ack_senddcc_str();
    }
  }
}

}  // namespace

/// DCC_EIN receive task function
///
/// Immediately suspends itself after creation. It's only resumed after
/// usb::rx_task_function receives a "DCC_EIN\r" protocol entry string. Once
/// running scan the CDC character stream for strings with pattern `senddcc(
/// [\d0-9a-fA-F]{2})+\r` and transmit the data to out::tx_message_buffer.
void rx_task_function(void*) {
  for (;;) {
    LOGI_TASK_SUSPEND(rx_task.handle);

    //
    if (auto expected{Mode::Suspended};
        mode.compare_exchange_strong(expected, Mode::DCC_EIN)) {
      transmit_ok();
      send_idle_packets_to_back();
      LOGI_TASK_RESUME(out::track::dcc::task.handle);
      loop();
    }
    //
    else
      transmit_not_ok();

    LOGI_TASK_RESUME(usb::rx_task.handle);
  }
}

}  // namespace usb::dcc_ein