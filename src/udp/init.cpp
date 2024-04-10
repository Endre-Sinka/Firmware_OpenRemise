/// Initialize UDP
///
/// \file   udp/init.cpp
/// \author Vincent Hamp
/// \date   04/04/2024

#include "init.hpp"
#include <lwip/err.h>
#include <lwip/netdb.h>
#include <lwip/sockets.h>
#include <lwip/sys.h>
#include "log.h"

namespace udp {

constexpr uint16_t port{21105u};

void task_function(void* pv_sock) {
  auto const sock{std::bit_cast<int>(pv_sock)};
  sockaddr addr;
  socklen_t socklen{sizeof(addr)};

  std::array<uint8_t, 1472uz> rx;
  std::array<char, 128uz> addr_str;

  for (;;) {
    auto const len{
      recvfrom(sock, data(rx), sizeof(rx) - 1, 0, &addr, &socklen)};

    // Error occurred during receiving
    if (len < 0) {
      LOGE("recvfrom failed: errno %d", errno);
      continue;
    }

    LOG_BUFFER_HEX_LEVEL(data(rx), len, ESP_LOG_INFO);
    printf("\n");
  }

  if (sock != -1) {
    LOGE("Shutting down socket and restarting...");
    shutdown(sock, 0);
    close(sock);
  }

  vTaskDelete(NULL);
}

///
esp_err_t init(BaseType_t xCoreID) {
  //
  auto sock{socket(AF_INET, SOCK_DGRAM, IPPROTO_IP)};
  assert(sock >= 0);
  LOGI("Socket created");

  //
  sockaddr_in6 dest_addr;
  sockaddr_in* dest_addr_ip4{std::bit_cast<sockaddr_in*>(&dest_addr)};
  dest_addr_ip4->sin_addr.s_addr = htonl(INADDR_ANY);
  dest_addr_ip4->sin_family = AF_INET;
  dest_addr_ip4->sin_port = htons(port);
  auto err{bind(sock, std::bit_cast<sockaddr*>(&dest_addr), sizeof(dest_addr))};
  assert(err >= 0);
  LOGI("Socket bound, port %d", port);

  assert(xTaskCreatePinnedToCore(task_function,
                                 task.name,
                                 task.stack_depth,
                                 std::bit_cast<void*>(sock),
                                 task.priority,
                                 &task.handle,
                                 xCoreID));

  return ESP_OK;
}

}  // namespace udp
