#include "service.hpp"
#include <lwip/sockets.h>
#include <ztl/string.hpp>
#include "log.h"
#include "utility.hpp"

namespace z21 {

using namespace std::literals;

/// TODO
Service::Service(BaseType_t xCoreID) {
  if (!xTaskCreatePinnedToCore(make_tramp(this, &Service::taskFunction),
                               task.name,
                               task.stack_depth,
                               NULL,
                               task.priority,
                               &task.handle,
                               xCoreID))
    assert(false);
}

/// TODO
Service::~Service() {
  if (task.handle) vTaskDelete(task.handle);
}

///
void Service::dcc(std::shared_ptr<z21::server::intf::Dcc> dcc_service) {
  _dcc_service = dcc_service;
}

/// TODO
esp_err_t Service::socket(http::Message& msg) {
  switch (msg.type) {
    case HTTPD_WS_TYPE_BINARY: {
      _ws_sock_fds.insert(msg.sock_fd);

      //
      sockaddr_in dest_addr_ip4;
      socklen_t socklen{sizeof(dest_addr_ip4)};
      if (getpeername(msg.sock_fd,
                      std::bit_cast<sockaddr*>(&dest_addr_ip4),
                      &socklen) < 0) {
        LOGE("getpeername failed %s", strerror(errno));
        return ESP_FAIL;
      }

      //
      std::lock_guard lock{_internal_mutex};
      receive({msg.sock_fd, std::bit_cast<sockaddr*>(&dest_addr_ip4), socklen},
              msg.payload);
      execute();
      break;
    }
    case HTTPD_WS_TYPE_CLOSE:
      // TODO implicit power off here if there are no more clients registered?
      _ws_sock_fds.erase(msg.sock_fd);
      break;
    default: break;
  }

  return ESP_OK;
}

/// TODO
void Service::taskFunction(void*) {
  std::array<uint8_t, Z21_MAX_PAYLOAD_SIZE> stack;
  sockaddr_in dest_addr_ip4;
  socklen_t socklen{sizeof(dest_addr_ip4)};

  /*
  // Multiple sockets, non-blocking ->
  fd_set fds;
  FD_ZERO(&fds);
  FD_SET(udp::sock_fd, &fds);

  select(FD_SETSIZE, &fds, NULL, NULL, NULL);

  if (FD_ISSET(udp::sock_fd, &fds)) {
    recvfrom...
  }
  */

  for (;;) {
    if (auto const len{recvfrom(udp::sock_fd,
                                data(stack),
                                size(stack) - 1,
                                0,
                                std::bit_cast<sockaddr*>(&dest_addr_ip4),
                                &socklen)};
        len < 0) {
      LOGE("recvfrom failed %s", strerror(errno));
      vTaskDelay(pdMS_TO_TICKS(task.timeout));
    } else if (len > 0) {
      std::lock_guard lock{_internal_mutex};
      receive({udp::sock_fd, std::bit_cast<sockaddr*>(&dest_addr_ip4), socklen},
              {data(stack), static_cast<size_t>(len)});
      execute();
    }
  }
}

///
void Service::transmit(z21::Socket const& sock,
                       std::span<uint8_t const> datasets) {
  //
  if (_ws_sock_fds.contains(sock.fd)) {
    httpd_ws_frame_t frame{
      .type = HTTPD_WS_TYPE_BINARY,
      .payload = std::bit_cast<uint8_t*>(data(datasets)),
      .len = size(datasets),
    };
    httpd_ws_send_frame_async(sock.fd, &frame);
  }
  //
  else {
    if (sendto(sock.fd,
               std::bit_cast<char*>(data(datasets)),
               size(datasets),
               0,
               std::bit_cast<sockaddr*>(&sock.addr),
               sock.len) < 0) {
      LOGE("sendto failed %s", strerror(errno));
      vTaskDelay(pdMS_TO_TICKS(task.timeout));
    }
  }
}

///
bool Service::trackPower(bool on) {
  return trackPower(on, Mode::DCCOperations);
}

///
bool Service::stop() {
  printf("%s\n", __PRETTY_FUNCTION__);
  return true;
}

///
void Service::logoff(z21::Socket const& sock) {
  printf("%s\n", __PRETTY_FUNCTION__);
  // TODO
  // broadcasting stop might be a good idea here...
  if (empty(clients())) trackPower(false);
}

///
z21::LocoInfo::Mode Service::locoMode(uint16_t addr) {
  return _dcc_service->locoMode(addr);
}

///
void Service::locoMode(uint16_t addr, z21::LocoInfo::Mode mode) {
  _dcc_service->locoMode(addr, mode);
}

///
void Service::function(uint16_t addr, uint32_t mask, uint32_t state) {
  _dcc_service->function(addr, mask, state);
}

///
void Service::drive(uint16_t addr,
                    z21::LocoInfo::SpeedSteps speed_steps,
                    uint8_t rvvvvvvv) {
  _dcc_service->drive(addr, speed_steps, rvvvvvvv);
}

///
z21::LocoInfo Service::locoInfo(uint16_t addr) {
  return _dcc_service->locoInfo(addr);
}

///
bool Service::cvRead(uint16_t cv_addr) {
  return trackPower(true, Mode::DCCService) && _dcc_service->cvRead(cv_addr);
}

///
bool Service::cvWrite(uint16_t cv_addr, uint8_t byte) {
  return trackPower(true, Mode::DCCService) &&
         _dcc_service->cvWrite(cv_addr, byte);
}

///
void Service::cvPomRead(uint16_t addr, uint16_t cv_addr) {
  _dcc_service->cvPomRead(addr, cv_addr);
}

///
void Service::cvPomWrite(uint16_t addr, uint16_t cv_addr, uint8_t byte) {
  _dcc_service->cvPomWrite(addr, cv_addr, byte);
}

///
bool Service::trackPower(bool on, Mode dcc_mode) {
  if (on) {
    switch (mode.load()) {
      // Wait for shutdown to complete
      case Mode::Shutdown:
        while (mode.load() != Mode::Suspended)
          vTaskDelay(pdMS_TO_TICKS(task.timeout));
        [[fallthrough]];

      //
      case Mode::Suspended:
        while (eTaskGetState(out::track::dcc::task.handle) != eSuspended)
          vTaskDelay(pdMS_TO_TICKS(task.timeout));
        if (auto expected{Mode::Suspended};
            mode.compare_exchange_strong(expected, dcc_mode))
          LOGI_TASK_RESUME(dcc::task.handle);
        else assert(false);
        [[fallthrough]];

      // Already running
      case Mode::DCCService: [[fallthrough]];
      case Mode::DCCOperations: return true;

      //
      default:
        // broadcast track power off here?
        // or maybe programming?
        return false;
    }
  } else {

    // This does... never... happen? TODO
    // Z21 app NEVER turn power off -.-
    if (auto op{Mode::DCCOperations}, serv{Mode::DCCService};
        mode.compare_exchange_strong(op, Mode::Shutdown) ||
        mode.compare_exchange_strong(serv, Mode::Shutdown))
      ;

    return true;
  }
}

}  // namespace z21