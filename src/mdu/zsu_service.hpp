#pragma once

#include <esp_task.h>
#include <array>
#include <queue>
#include "http/message.hpp"

namespace mdu {

class ZsuService {
public:
  esp_err_t zsuSocket(http::Message& msg);

protected:
  void loop();

private:
  std::array<uint8_t, 2uz> transmit(std::vector<uint8_t> const& payload) const;
  void close();

  std::queue<http::Message> _queue{};
  std::array<uint8_t, 2uz> _acks{};
};

}  // namespace mdu