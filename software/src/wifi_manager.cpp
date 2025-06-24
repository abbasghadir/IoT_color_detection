#include "wifi_manager.h"

extern CONFIG_SETTING config_setting;

IPAddress AP_IP(192, 168, 4, 1);
IPAddress AP_gateway(192, 168, 1, 1);
IPAddress AP_subnet(255,255,255,0);
IPAddress STA_IP,STA_gateway,STA_subnet;


void wificonfig(){
  STA_IP.fromString(config_setting.subnet_mask);
  STA_gateway.fromString(config_setting.default_gateway);
  STA_subnet.fromString(config_setting.Local_IP_address);
  WiFi.config(STA_IP, STA_gateway, STA_subnet);
}

void WiFiStartStation(){
    WiFi.disconnect();
    WiFi.mode(WIFI_STA);
    wificonfig();
    WiFi.begin(config_setting.ssid_STA.c_str(),config_setting.password_STA.c_str());
    int loopbreaker = 0;
    while (1)
    {
        if (WiFi.status() != WL_CONNECTED)
        {
            delay(500);
            Serial.print(".");
        }else if (WiFi.status() == WL_CONNECTED)
        {
            Serial.print("\nwifi connected to:");
            Serial.println(WiFi.localIP());
            break;
        }
        loopbreaker ++;
        if (loopbreaker>=50)
        {
            ESP.restart();
        }
    }    
}

void WiFiStartAccessPoint(){
    WiFi.disconnect();
    WiFi.mode(WIFI_AP);
    WiFi.softAP(config_setting.ssid_AP.c_str(),config_setting.password_AP.c_str());
    WiFi.softAPConfig(AP_IP,AP_gateway,AP_subnet);
    Serial.println("Access Point Started.");
    Serial.print("IP Address: ");
    Serial.println((String)(config_setting.ssid_STA));
    Serial.println((String)(config_setting.password_STA));
    Serial.println(WiFi.softAPIP());
}