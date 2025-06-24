#ifndef WEBSERVER_MANAGER_H
#define WEBSERVER_MANAGER_H
#include <WiFi.h>
#include <WebServer.h>
#include "Storage_manager.h"
#include "camera_manager.h"

void ServerAccessPoint();
void handleAPLoadSettings();
void handleAPSaveSettings();
void handleLoginPage();
void handleAPSettingsPage();
void handleLoginCredential();

void ServerStationMode();
void handleWifiPage();
void handleswitchLoginPage();
void handleswitchLoginCredential();
void switcheruser();
void handleWifiSaveUsers();
void handleWifiLoadUsers();
void handleWifiStartMQTTConnection();
void handleWifiStopMQTTConnection();

void camera_show();

void handleWebServerclient();

void restartNodeMCU();
#endif