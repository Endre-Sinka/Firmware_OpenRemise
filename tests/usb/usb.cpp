#include "usb/rx_task_function.hpp"
#include "usb_test.hpp"

// UsbTest ctor starts all protocol tasks, so any_protocol_task_active must
// return true
TEST_F(UsbTest, all_protocol_tasks_active) {
  EXPECT_LT(eTaskGetState(usb::dcc_ein::rx_task.handle), eSuspended);
  EXPECT_LT(eTaskGetState(usb::dcc_ein::tx_task.handle), eSuspended);
  EXPECT_LT(eTaskGetState(usb::susiv2::rx_task.handle), eSuspended);
  EXPECT_LT(eTaskGetState(usb::susiv2::tx_task.handle), eSuspended);
  EXPECT_TRUE(usb::any_protocol_task_active());
}

// After suspending all protocol tasks any_protocol_task_active must return
// false
TEST_F(UsbTest, all_protocol_tasks_suspended) {
  vTaskSuspend(usb::dcc_ein::rx_task.handle);
  vTaskSuspend(usb::dcc_ein::tx_task.handle);
  vTaskSuspend(usb::susiv2::rx_task.handle);
  vTaskSuspend(usb::susiv2::tx_task.handle);
  EXPECT_EQ(eTaskGetState(usb::dcc_ein::rx_task.handle), eSuspended);
  EXPECT_EQ(eTaskGetState(usb::dcc_ein::tx_task.handle), eSuspended);
  EXPECT_EQ(eTaskGetState(usb::susiv2::rx_task.handle), eSuspended);
  EXPECT_EQ(eTaskGetState(usb::susiv2::tx_task.handle), eSuspended);
  EXPECT_FALSE(usb::any_protocol_task_active());
}