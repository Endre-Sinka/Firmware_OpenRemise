/// DCC_EIN protocol transmit task function
///
/// \file   usb/dcc_ein/tx_task_function.cpp
/// \author Vincent Hamp
/// \date   10/02/2023

#include "tx_task_function.hpp"
#include <algorithm>
#include <charconv>
#include <span>
#include <string_view>
#include <ulf/dcc_ein.hpp>
#include "log.h"

namespace usb::dcc_ein {

namespace {

/// Receive addressed datagram
///
/// \return Addressed BiDi datagram received from out::track::rx_queue
/// \return std::nullopt on timeout
std::optional<ulf::dcc_ein::AddressedDatagram> receive_addressed_datagram() {
  out::track::RxQueue::value_type item;
  if (xQueueReceive(
        out::track::rx_queue.handle, &item, pdMS_TO_TICKS(tx_task.timeout)))
    return ulf::dcc_ein::AddressedDatagram{
      .addr = dcc::decode_address(item.packet), .datagram = item.datagram};
  else return std::nullopt;
}

/// Transmit addressed datagram
///
/// The addressed datagram is converted to a sendbidi string with pattern
/// `sendbidi [ubsalrtei][0-9a-fA-F]{4}( [0-9a-fA-F]{2}){8}\r` prior to
/// transmission. The string is then send to usb::tx_stream_buffer.
///
/// \param  addr_datagram Addressed BiDi datagram received from
///                       out::track::rx_queue
void transmit_addressed_datagram(
  ulf::dcc_ein::AddressedDatagram const& addr_datagram) {
  auto const str{ulf::dcc_ein::addressed_datagram2sendbidi_str(addr_datagram)};
  xStreamBufferSend(tx_stream_buffer.handle,
                    data(str),
                    size(str),
                    pdMS_TO_TICKS(tx_task.timeout));
}

/// Actual usb::dcc_ein::tx_task loop
void loop() {
  while (eTaskGetState(rx_task.handle) < eSuspended)
    if (auto const addr_datagram{receive_addressed_datagram()};
        addr_datagram && std::ranges::any_of(addr_datagram->datagram,
                                             [](uint8_t b) { return b; }))
      transmit_addressed_datagram(*addr_datagram);
}

}  // namespace

/// DCC_EIN transmit task function
///
/// Immediately suspends itself after creation. It's only resumed after
/// usb::rx_task_function receives a "DCC_EIN\r" protocol entry string.
void tx_task_function(void*) {
  for (;;) {
    LOGI_TASK_SUSPEND(tx_task.handle);

    loop();

    LOGI_TASK_RESUME(usb::rx_task.handle);
  }
}

}  // namespace usb::dcc_ein