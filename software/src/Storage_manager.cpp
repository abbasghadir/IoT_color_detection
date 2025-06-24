#include "Storage_manager.h"

#define CONFIG_FILE_MCU "/MCU_config.txt"
#define CONFIG_FILE_USER "/USER_config.txt"
#define PUBLISH_TO_DB 1
#define PUBLISH_TO_BROKER 2
#define PUBLISH_TO_DB_AND_BROKER 3
#define default_ap_ssidText "NodeMCU-AP"
#define default_ap_passwordText "123456789"
#define default_loginUsernameText "admin"
#define default_loginPasswordText "admin"
#define default_publishToDBOrBrokerComboBoxValue PUBLISH_TO_DB
#define default_batchSizeToSendToDBComboBoxValue 10
#define default_timeIntervalToSendToDBComboBoxValue 15
#define default_mqttBrokerPortValue 1883
#define default_ntpSyncIntervalComboBoxValue 2

#pragma region internal_setting_file_configuration
void setDefaultSettings(CONFIG_SETTING &config_setting) {
  config_setting.ssid_AP = default_ap_ssidText;
  config_setting.password_AP = default_ap_passwordText;
  config_setting.loginUsername = default_loginUsernameText;
  config_setting.loginPassword = default_loginPasswordText;
  config_setting.ssid_STA = "";
  config_setting.password_STA = "";
  config_setting.publishToDBOrBrokerComboBoxValue = default_publishToDBOrBrokerComboBoxValue;
  config_setting.phpServerLink = "";
  config_setting.phpApiKeyValue = "";
  config_setting.batchSizeToSendToDBComboBoxVal = default_batchSizeToSendToDBComboBoxValue;
  config_setting.timeIntervalToSendToDBComboBoxVal = default_timeIntervalToSendToDBComboBoxValue;
  config_setting.mqttBrokerIP = "";
  config_setting.mqttBrokerPort = default_mqttBrokerPortValue;
  config_setting.mqttBrokerUser = "";
  config_setting.mqttBrokerPassword = "";
  config_setting.mqttClickEventTopic = "";
  config_setting.ntpServerIP = "";
  config_setting.ntpSyncIntervalComboBoxVal = default_ntpSyncIntervalComboBoxValue;
}

String removeNewline(String inputString) {
  inputString.trim();
  inputString.replace("\n", "");
  return inputString;
}

int loadconfig_MCU(CONFIG_SETTING &config_setting){
    setDefaultSettings(config_setting);
    if (!LittleFS.exists(CONFIG_FILE_MCU)) {
        Serial.println("Settings file not exists");
        return 1;   // The error number typically associated with this error is 2
    }

    File file = LittleFS.open(CONFIG_FILE_MCU, "r");
    if (!file) {
        Serial.println("Failed to load MCU config file.");
        // return 500;
    }
    String line;
    int switch_counter = 0;
    while (file.available()) {
        line = file.readStringUntil('\n');
        line = removeNewline(line.c_str());
        switch (switch_counter) {
        case 0:
            config_setting.ssid_AP = line;
            break;
        case 1:
            config_setting.password_AP = line;
            break;
        case 2:
            config_setting.loginUsername = line;
            break;
        case 3:
            config_setting.loginPassword = line;
            break;
        case 4:
            config_setting.ssid_STA = line;
            break;
        case 5:
            config_setting.password_STA = line;
            break;
        case 6:
            config_setting.publishToDBOrBrokerComboBoxValue = line.toInt();
            break;
        case 7:
            config_setting.phpServerLink = line;
            break;
        case 8:
            config_setting.phpApiKeyValue = line;
            break;
        case 9:
            config_setting.batchSizeToSendToDBComboBoxVal = line.toInt();
            break;
        case 10:
            config_setting.timeIntervalToSendToDBComboBoxVal = line.toInt();
            break;
        case 11:
            config_setting.mqttBrokerIP = line;
            break;
        case 12:
            config_setting.mqttBrokerPort = line.toInt();
            break;
        case 13:
            config_setting.mqttBrokerUser = line;
            break;
        case 14:
            config_setting.mqttBrokerPassword = line;
            break;
        case 15:
            config_setting.mqttClickEventTopic = line;
            break;
        case 16:
            config_setting.ntpServerIP = line;
            break;
        case 17:
            config_setting.ntpSyncIntervalComboBoxVal = line.toInt();
            break;
        case 18:
            config_setting.Local_IP_address = line;
                break;
        case 19:
            config_setting.default_gateway = line;
            break;
        case 20:
            config_setting.subnet_mask = line;
            break;
        }
        switch_counter++;
    }
    file.close();
    Serial.println("Load successfully");
    return 2;
}


bool saveconfig_MCU(CONFIG_SETTING &config_setting){
    File file = LittleFS.open(CONFIG_FILE_MCU, "w");
    if (!file) {
        Serial.println("Failed to save MCU config file.");
        return false;
    }
    file.println(config_setting.ssid_AP);
    file.println(config_setting.password_AP);
    file.println(config_setting.loginUsername);
    file.println(config_setting.loginPassword);
    file.println(config_setting.ssid_STA);
    file.println(config_setting.password_STA);
    file.println(config_setting.publishToDBOrBrokerComboBoxValue);
    file.println(config_setting.phpServerLink);
    file.println(config_setting.phpApiKeyValue);
    file.println(config_setting.batchSizeToSendToDBComboBoxVal);
    file.println(config_setting.timeIntervalToSendToDBComboBoxVal);
    file.println(config_setting.mqttBrokerIP);
    file.println(config_setting.mqttBrokerPort);
    file.println(config_setting.mqttBrokerUser);
    file.println(config_setting.mqttBrokerPassword);
    file.println(config_setting.mqttClickEventTopic);
    file.println(config_setting.ntpServerIP);
    file.println(config_setting.ntpSyncIntervalComboBoxVal);
    file.println(config_setting.Local_IP_address);
    file.println(config_setting.default_gateway);
    file.println(config_setting.subnet_mask);    
    file.close();
    return true;
}
#pragma endregion
#pragma region user_setting_file_configuration
int loadconfig_user(USER_SETTING &user_setting){
    if (!LittleFS.exists(CONFIG_FILE_USER)) {
        Serial.println("Users file not exists.");
        return 2;
    }

    File file = LittleFS.open(CONFIG_FILE_USER, "r");
    if (!file) {
        Serial.println("Failed to load User config file.");
        return 0;
    }

    // user_setting.usernameText    = file.readStringUntil('\n');
    user_setting.descriptionText = file.readStringUntil('\n');
    user_setting.Produt_nameText = file.readStringUntil('\n');
    file.close();
    return 1;
}

bool saveconfig_user(USER_SETTING &user_setting){
    Serial.println("1");
    File file = LittleFS.open(CONFIG_FILE_USER, "w");
    if (!file) {
        Serial.println("Failed to save USER config file.");
        return false;
    }
    // file.println(user_setting.usernameText);
    file.println(user_setting.descriptionText);
    file.println(user_setting.Produt_nameText);
    file.close();
    return true;
}
#pragma endregion

void littlefs_init(){
    if (!LittleFS.begin(true))
    {
        Serial.println("Failed to initialize LittleFS");
        return;
    }
}