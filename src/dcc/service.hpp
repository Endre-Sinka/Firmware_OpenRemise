#pragma once

#include <mutex>
#include <optional>
#include "operations_service.hpp"
#include "service_service.hpp"

namespace dcc {

class Service : public OperationsService, public ServiceService {
public:
  explicit Service(BaseType_t xCoreID);
  ~Service();

private:
  // This gets called by FreeRTOS
  void taskFunction(void*);

  void loop();
};

}  // namespace dcc