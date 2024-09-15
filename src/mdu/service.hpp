#pragma once

#include <esp_task.h>
#include <queue>
#include "zpp_service.hpp"
#include "zsu_service.hpp"

namespace mdu {

class Service : public ZppService, public ZsuService {
public:
  explicit Service(BaseType_t xCoreID);
  ~Service();

private:
  // This gets called by FreeRTOS
  void taskFunction(void*);
};

}  // namespace mdu