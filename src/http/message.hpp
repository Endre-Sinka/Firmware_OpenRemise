/// HTTP websocket message
///
/// \file   http/message.hpp
/// \author Vincent Hamp
/// \date   12/04/2023

#pragma once

#include <esp_http_server.h>
#include <vector>

namespace http {

struct Message {
  int sock_fd;
  httpd_ws_type_t type;
  std::vector<uint8_t> payload;
};

}  // namespace http