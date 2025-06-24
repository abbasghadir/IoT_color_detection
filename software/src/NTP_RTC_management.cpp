#include "NTP_RTC_management.h"

extern CONFIG_SETTING config_setting;

const char* ntpserver = config_setting.ntpServerIP.c_str();
const long utcOffsetInSeconds = 12600; //for iran time +3:30
unsigned long previousTimeNTPSync = millis();
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, ntpserver, utcOffsetInSeconds);


void set_RTC(time_t epochTime){
    struct timeval tv;
    tv.tv_sec = epochTime;
    tv.tv_usec = 0;
    settimeofday(&tv, NULL);
    Serial.println("RTC set successfully!");
}

void NTP_init(){
    timeClient.begin();
    timeClient.forceUpdate();
    time_t epoch = timeClient.getEpochTime();
    set_RTC(epoch);
}

void get_time_date(String &local_time, String &local_date){//char *local_date, char *local_time){

  time_t now = time(NULL);
  struct tm *timeinfo = localtime(&now);
  // Serial.println("RTC time:");
  local_time = timeinfo->tm_hour;
  local_time += ":";
  local_time += timeinfo->tm_min;
  local_time += ":";
  local_time += timeinfo->tm_sec;
  local_date = timeinfo->tm_year+1900;
  local_date += ":";
  local_date += timeinfo->tm_mon+1;
  local_date += ":";
  local_date += timeinfo->tm_mday;
}

void NTP_update(){
  if (millis() - previousTimeNTPSync>=(config_setting.ntpSyncIntervalComboBoxVal.toInt() * 60 * 1000))
  {
    previousTimeNTPSync = millis();
    timeClient.forceUpdate();
    time_t epoch = timeClient.getEpochTime();
    Serial.println(config_setting.ntpSyncIntervalComboBoxVal.toInt());
    set_RTC(epoch);
  }
}