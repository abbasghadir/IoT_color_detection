#ifndef STORAGE_MANAGER_H
#define STORAGE_MANAGER_H
#include <LittleFS.h>
#include <Arduino.h>
struct CONFIG_SETTING{
    String ssid_AP;
    String password_AP;
    String loginUsername;
    String loginPassword;
    String ssid_STA;
    String password_STA;
    String publishToDBOrBrokerComboBoxValue;
    String phpServerLink;
    String phpApiKeyValue;
    String batchSizeToSendToDBComboBoxVal;
    String timeIntervalToSendToDBComboBoxVal;
    String mqttBrokerIP;
    String mqttBrokerPort;
    String mqttBrokerUser;
    String mqttBrokerPassword;
    String mqttClickEventTopic;
    String ntpServerIP;
    String ntpSyncIntervalComboBoxVal;
    String Local_IP_address;
    String default_gateway;
    String subnet_mask;
};

struct USER_SETTING{
    // String usernameText;
    String descriptionText;
    String Produt_nameText;
};

int loadconfig_MCU(CONFIG_SETTING &config_setting);
bool saveconfig_MCU(CONFIG_SETTING &config_setting);

int loadconfig_user(USER_SETTING &user_setting);
bool saveconfig_user(USER_SETTING &user_setting);

void littlefs_init();

String removeNewline(String inputString);
#endif