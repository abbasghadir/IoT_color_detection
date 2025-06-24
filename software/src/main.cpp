#include "color_detection_manager.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "wifi_manager.h"
#include "webserver_manager.h"
#include "Storage_manager.h"
#include "camera_manager.h"
#include "interrupt_manager.h"
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "NTP_RTC_management.h"
#include "data_transfer_management.h"

String press_result = "false";
bool color_detection_start = false;

#define AP_mode 1
#define STA_mode 2

ROI_config ROI;
CONFIG_SETTING config_setting;

void setup(){    
    Serial.begin(115200);
    ROI.buffer = NULL;
    ROI.x = 0;
    ROI.y = 0;
    ROI.x_width = 100;
    ROI.y_height = 80;
    ROI.BPPN = 2; // bytes_per_pixel_number
    camera_init();    
    littlefs_init();
    interrupt_init();
    int state = loadconfig_MCU(config_setting);
    // camera_show();
    switch (state)
    {
    case AP_mode:
        WiFiStartAccessPoint();
        ServerAccessPoint();
        color_detection_start = false;
        break;
    case STA_mode:
        WiFiStartStation();
        ServerStationMode();
        color_detection_start = true;
        xTaskCreate(camera_capture, "camera_capture",8192, NULL, 1, nullptr);
        // xTaskCreate(color_range_detection, "color_range_detection",8192, NULL, 1, nullptr);
        break;
    }
    NTP_init();
}

void loop(){
    handleWebServerclient();
    if (color_detection_start)
    {
        NTP_update();
        // camera_capture();
        // color_range_detection();
        publishClickEventMessage(press_result);
    }
    delay(10);
}

