#ifndef DATA_TRANSFER_MANAGEMENT_H
#define DATA_TRANSFER_MANAGEMENT_H
#include "webserver_manager.h"
#include "Storage_manager.h"
#include "NTP_RTC_management.h"
#include "webserver_manager.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>

// Define a struct to represent a log entry
struct LogEntry {
  String username;
  String user_description;
  String log_date;
  String log_time;
  String product_number_value;
  String press_result;
};

void publishClickEventMessage(String press_result_value);

#endif