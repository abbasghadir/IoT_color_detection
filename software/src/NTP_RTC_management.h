#ifndef NTP__RTC_MANAGEMENT_H
#define NTP__RTC_MANAGEMENT_H
#include <WiFiUdp.h>
#include <NTPClient.h>
#include "Storage_manager.h"

void NTP_init();
void get_time_date(String &local_time, String &local_date);
void NTP_update();

#endif