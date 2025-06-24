#include "webserver_manager.h"

WebServer server(80);
extern CONFIG_SETTING config_setting;
extern ROI_config ROI;
USER_SETTING user_setting;

String htmlMessageInfo = "";
String marqueeHtml = "<marquee direction='right' scrollamount='5' behavior='scroll'><p style='font-weight:bold; color:green;'>";
String marqueeEndHtml = "</P></marquee>";

bool Switchnumber = true;
String Switchtext1[2] = {"User","Admin"};
const int NumberOfStation  = 1;
const int operator_number = 11;
String Switchtext2[2] = {"readonly",""};
String usernameText[7] = { "1", "2", "3", "4", "5", "6", "7" };
String descriptionText[7] = { "", "", "", "", "", "", "" };
String Produt_nameText[7] ={ "", "", "", "", "", "", "" };
String Operator1[operator_number] = {"","1","2","3","4","5","6","7","8","9","10"};
String Operator2[operator_number] = {"","1","2","3","4","5","6","7","8","9","10"};
String Operator3[operator_number] = {"","1","2","3","4","5","6","7","8","9","10"};
String Operator4[operator_number] = {"","1","2","3","4","5","6","7","8","9","10"};
String Operator5[operator_number] = {"","1","2","3","4","5","6","7","8","9","10"};
String *Operators[5] = {Operator1, Operator2, Operator3, Operator4, Operator5};
String operatorname[operator_number];
bool pagecheck = false;


#pragma region Access_point_setting_page
// Access Point mode
void ServerAccessPoint() {
    // server.stop();
  // When we switch from station mode to acces point mode, the web server
  // may not be properly closed or reconfigured. To resolve this issue
  // we need to explicitly stop the web server before switching.
  // Setup the server
  server.on("/", handleLoginPage);
  server.on("/loginCheck", handleLoginCredential);
  server.on("/saveSettings", HTTP_GET, handleAPSaveSettings);
  server.on("/loadSettings", HTTP_GET, handleAPLoadSettings);
  server.on("/restart", HTTP_GET, restartNodeMCU);

  // Start the server
  server.begin();
  // The WiFi.softAP() function does not immediately establish a connection to the access point.
  // To deal with the delay in obtaining the IP address, you can listen for the softAPMode
  // connected event before getting the local IP address.
  // ToDo, check the logic above; it should print the line below!
  Serial.println("WiFi.localIP -> 192.168.4.1");
  Serial.println("Access Point ready. Connect to NodeMCU-Access-Point to change settings.");
  // Additional settings for access point if needed
}

// Load Settings to fill the html
void handleAPLoadSettings() {
    loadconfig_MCU(config_setting);
    Serial.print("wifi_ssid= ");
    Serial.println(config_setting.ssid_STA);
    Serial.print("wifi_password= ");
    Serial.println(config_setting.password_STA);
    Serial.print("phpApiKeyValueText= ");
    Serial.println(config_setting.phpApiKeyValue);
    Serial.print("timeIntervalToSendToDBComboBoxVal= ");
    Serial.println(config_setting.timeIntervalToSendToDBComboBoxVal);
    String message = "Settings loaded!";
    htmlMessageInfo = "<label style='font-weight:bold; color:green;'>";
    htmlMessageInfo += message;
    htmlMessageInfo += "</label>";
    htmlMessageInfo += marqueeHtml;
    htmlMessageInfo += message;
    htmlMessageInfo += marqueeEndHtml;
    Serial.println(message);
    handleAPSettingsPage();
}

// Save Settings in a file
void handleAPSaveSettings() {
    if (server.args() == 0) {
        server.send(400, "text/plain", "Invalid request");
        return;
    }
    config_setting.ssid_AP = server.arg("ap_ssid");
    config_setting.password_AP = server.arg("ap_password");
    config_setting.loginUsername = server.arg("login_username");
    config_setting.loginPassword = server.arg("login_password");
    config_setting.ssid_STA = server.arg("wifi_ssid");
    config_setting.password_STA = server.arg("wifi_password");
    config_setting.publishToDBOrBrokerComboBoxValue = server.arg("publishToDBOrBrokerComboBox");
    config_setting.phpServerLink = server.arg("phpServerLink");
    config_setting.phpApiKeyValue = server.arg("phpApiKeyValue");
    config_setting.batchSizeToSendToDBComboBoxVal = server.arg("batchSizeToSendToDBComboBox");
    config_setting.timeIntervalToSendToDBComboBoxVal = server.arg("timeIntervalToSendToDBComboBox");
    config_setting.mqttBrokerIP = server.arg("mqttBrokerIP");
    config_setting.mqttBrokerPort = server.arg("mqttBrokerPort");
    config_setting.mqttBrokerUser = server.arg("mqttBrokerUser");
    config_setting.mqttBrokerPassword = server.arg("mqttBrokerPassword");
    config_setting.mqttClickEventTopic = server.arg("mqttClickTopic");
    config_setting.ntpServerIP = server.arg("ntpServerIP");
    config_setting.ntpSyncIntervalComboBoxVal = server.arg("ntpSyncIntervalComboBox");
    config_setting.Local_IP_address = server.arg("Local_IP_address");
    config_setting.default_gateway = server.arg("default_gateway");
    config_setting.subnet_mask = server.arg("subnet_mask");

    saveconfig_MCU(config_setting);
    
    Serial.print("wifi_ssid= ");
    Serial.println(config_setting.ssid_STA);
    Serial.print("wifi_password= ");
    Serial.println(config_setting.password_STA);
    Serial.print("phpApiKeyValueText= ");
    Serial.println(config_setting.phpApiKeyValue);
    Serial.print("timeIntervalToSendToDBComboBoxVal= ");
    Serial.println(config_setting.timeIntervalToSendToDBComboBoxVal);
    String message = "Data saved successfully!";
    htmlMessageInfo = "<label style='font-weight:bold; color:green;'>";
    htmlMessageInfo += message;
    htmlMessageInfo += "</label>";
    htmlMessageInfo += marqueeHtml;
    htmlMessageInfo += message;
    htmlMessageInfo += marqueeEndHtml;
    Serial.println(message);
    handleAPSettingsPage();
}

void handleLoginPage() {
    String loginPage = "<html><body>";
    loginPage += "<style> input[type='text'] { padding: 5px; margin: 5px; }  ";
    loginPage += "input[type='password'] { padding: 5px; margin: 5px; }  ";
    loginPage += "</style>";
    loginPage += "<h1 style='text-align:center;'>Click Handler Web Server - Login</h1>";
    loginPage += "<div style='text-align:center;'>";
    loginPage += "<form action='/loginCheck' method='POST'>";
    loginPage += "<label for='login_username'>Username:</label>";
    loginPage += "<input id='login_username' name='login_username' minlength='3' type='text' ><br>";
    loginPage += "<label for='login_password'>Password:</label>";
    loginPage += "<input id='login_password' name='login_password' minlength='3' type='password' ><br>";
    loginPage += "<button style='margin: 7px; background-color: LightGray;' type='submit'>Login</button>";
    loginPage += "</form>";
    loginPage += "</div>";
    loginPage += "</body></html>";
    server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    server.sendHeader("Pragma", "no-cache");
    server.sendHeader("Expires", "-1");
    server.send(200, "text/html", loginPage);
}

// Access Point Settings page
void handleAPSettingsPage() {
    String apPage = "<html><body>";
    apPage += "<style>";
    apPage += "input[type='text'] { padding: 5px; margin: 5px; }  ";
    apPage += "input[type='password'] { padding: 5px; margin: 5px; }  ";
    apPage += "select { padding: 5px; margin: 5px; }  ";
    apPage += "h4 { text-decoration: underline; } ";
    apPage += ".peach { background-color: #FFE5B4; padding: 10px; margin-bottom: 5px; } ";
    apPage += ".purple { background-color: #ce95fc; padding: 10px; margin-bottom: 5px; } ";
    apPage += ".LightBlue { background-color: LightBlue; padding: 10px; margin-bottom: 5px; } ";
    apPage += ".LightYellow { background-color: LightYellow; padding: 10px; margin-bottom: 5px; } ";
    apPage += ".LightGreen { background-color: LightGreen; padding: 10px; margin-bottom: 5px; } ";
    apPage += "</style>";
    apPage += "<h1 style='text-align:center;'>Click Handler Web Server - Settings</h1>";
    apPage += "<form>";
    apPage += "<div class='LightBlue'>";
    apPage += "<h4>Access Point</h4>";
    apPage += "<label for='ap_ssid'>AP SSID:</label>";
    apPage += "<input type='text' id='ap_ssid' name='ap_ssid' placeholder='Enter AP SSID' minlength='3' ";
    apPage += " value='" + config_setting.ssid_AP + "'>";
    apPage += "<label for='ap_password'>AP Password:</label>";
    apPage += "<input type='password' id='ap_password' name='ap_password' placeholder='Enter AP Password' minlength='5'";
    apPage += " value='" + config_setting.password_AP + "'>";
    apPage += "<input type='checkbox' onclick='showPasswordFunction()'>Show Password<br>";
    apPage += "<label for='login_username'>Username:</label>";
    apPage += "<input id='login_username' name='login_username' minlength='3' placeholder='Enter Username' type='text' ";
    apPage += " value='" + config_setting.loginUsername + "'>";
    apPage += "<label for='login_password'>Password:</label>";
    apPage += "<input id='login_password' name='login_password' minlength='3' placeholder='Enter Password' type='password' ";
    apPage += " value='" + config_setting.loginPassword + "'><br>";
    apPage += "</div>";
    apPage += "<div class='peach'>";
    apPage += "<h4>WiFi</h4>";
    apPage += "<label for='wifi_ssid'>WiFi SSID:</label>";
    apPage += "<input type='text' id='wifi_ssid' name='wifi_ssid' placeholder='Enter WiFi SSID' minlength='3' ";
    apPage += " value='" + config_setting.ssid_STA + "'>";
    apPage += "<label for='wifi_password'>WiFi Password:</label>";
    apPage += "<input type='password' id='wifi_password' name='wifi_password' placeholder='Enter WiFi Password' minlength='5'";
    apPage += " value='" + config_setting.password_STA + "'>";
    apPage += "<input type='checkbox' onclick='showPasswordFunction()'>Show Password<br>";
    apPage += "</div>";
    apPage += "<div class='purple'>";
    apPage += "<h4>Static IP address setting</h4>";
    apPage += "<label for='check_box'>Enable setting:</label>";
    apPage += "<input type='checkbox' id='toggleCheckbox' onchange='toggleInput()'><br><br>";
    apPage += "<label for='Local_IP_address'>Local IP address:</label>";
    apPage += "<input type='text' id='Local_IP_address' name='Local_IP_address' placeholder='Enter IP' disabled";
    apPage += " value='" + config_setting.Local_IP_address + "'>";
    apPage += "<label for='default_gateway'>Default Gateway:</label>";
    apPage += "<input type='text' id='default_gateway' name='default_gateway' placeholder='Enter gateway' disabled";
    apPage += " value='" + config_setting.default_gateway + "'>";
    apPage += "<label for='subnet_mask'>Subnet mask</label>";
    apPage += "<input type='text' id='subnet_mask' name='subnet_mask' placeholder='Enter subnet' disabled";
    apPage += " value='" + config_setting.subnet_mask + "'>";
    apPage += "</div>";
    apPage += "<div class='LightGreen'>";
    apPage += "<h4>PHP & Database</h4>";
    apPage += "<label for='publishToDBOrBroker'>Publish to DB or Broker:</label>";
    apPage += "<select name='publishToDBOrBrokerComboBox'>";
    String whatShowsInComboBox = "";
    for (int i = 1; i <= 3; i ++) {
        if (i == 1)
        whatShowsInComboBox = "DB";
        else if (i == 2)
        whatShowsInComboBox = "Broker";
        else if (i == 3)
        whatShowsInComboBox = "DB and Broker";

        if (i == config_setting.publishToDBOrBrokerComboBoxValue.toInt()) {
        apPage += "<option value='" + String(i) + "' selected>" + whatShowsInComboBox + "</option>";
        } else {
        apPage += "<option value='" + String(i) + "'>" + whatShowsInComboBox + "</option>";
        }
  }
    apPage += "</select>";
    apPage += "<label for='phpServerLink'>PHP Server Link:</label>";
    apPage += "<input type='text' id='phpServerLink' name='phpServerLink' placeholder='Enter PHP Server Link' minlength='5' ";
    apPage += " value='" + config_setting.phpServerLink + "'>";
    apPage += "<label for='phpApiKeyValue'>PHP Api Key Value:</label>";
    apPage += "<input type='text' id='phpApiKeyValue' name='phpApiKeyValue' placeholder='Enter PHP Api Key Value' ";
    apPage += " value='" + config_setting.phpApiKeyValue + "'><br>";
    apPage += "<label for='batchSizeToSendToDB'>Batch Size to Send to DB:</label>";
    apPage += "<select name='batchSizeToSendToDBComboBox'>";
    for (int i = 10; i <= 20; i += 5) {
        if (i == config_setting.batchSizeToSendToDBComboBoxVal.toInt()) {
        apPage += "<option value='" + String(i) + "' selected>" + String(i) + "</option>";
        } else {
        apPage += "<option value='" + String(i) + "'>" + String(i) + "</option>";
        }
    }
    apPage += "</select>";
    apPage += "<label for='timeIntervalToSendToDB'>Time Interval to Send to DB:</label>";
    apPage += "<select name='timeIntervalToSendToDBComboBox'>";
    whatShowsInComboBox = "";
    for (int i = 15; i <= 60; i += 15) {
        if (i == 15)
        whatShowsInComboBox = "15 minutes";
        else if (i == 30)
        whatShowsInComboBox = "30 minutes";
        else if (i == 45)
        whatShowsInComboBox = "45 minutes";
        else if (i == 60)
        whatShowsInComboBox = "1 hour";

        if (i == config_setting.timeIntervalToSendToDBComboBoxVal.toFloat()) {
        apPage += "<option value='" + String(i) + "' selected>" + whatShowsInComboBox + "</option>";
        } else {
        apPage += "<option value='" + String(i) + "'>" + whatShowsInComboBox + "</option>";
        }
  }
    apPage += "</select><br>";
    apPage += "</div>";
    apPage += "<div class='LightYellow'>";
    apPage += "<h4>MQTT Broker</h4>";
    apPage += "<label for='mqttBrokerIP'>MQTT Broker IP:</label>";
    apPage += "<input type='text' id='mqttBrokerIP' name='mqttBrokerIP' placeholder='Enter MQTT Broker IP' ";
    apPage += " value='" + config_setting.mqttBrokerIP + "'>";
    apPage += "<label for='mqttBrokerPort'>MQTT Broker Port:</label>";
    apPage += "<input type='text' id='mqttBrokerPort' name='mqttBrokerPort' placeholder='Enter MQTT Broker Port' ";
    apPage += " value='" + String(config_setting.mqttBrokerPort) + "'>";
    apPage += "<label for='mqttBrokerUser'>MQTT Broker User:</label>";
    apPage += "<input type='text' id='mqttBrokerUser' name='mqttBrokerUser' placeholder='Enter MQTT Broker User'";
    apPage += " value='" + config_setting.mqttBrokerUser + "'><br>";
    apPage += "<label for='mqttBrokerPassword'>MQTT Broker Password:</label>";
    apPage += "<input type='text' id='mqttBrokerPassword' name='mqttBrokerPassword' placeholder='Enter MQTT Broker Password' ";
    apPage += " value='" + config_setting.mqttBrokerPassword + "'>";
    apPage += "<label for='mqttClickEventTopic'>MQTT Click Event Topic:</label>";
    apPage += "<input type='text' id='mqttClickEventTopic' name='mqttClickEventTopic' placeholder='Enter MQTT Click Event Topic'";
    apPage += " value='" + config_setting.mqttClickEventTopic + "'><br>";
    apPage += "</div>";
    apPage += "<div class='LightBlue'>";
    apPage += "<h4>NTP (Network Time Protocol)</h4>";
    apPage += "<label for='ntpServerIP'>NTP Server IP:</label>";
    apPage += "<input type='text' id='ntpServerIP' name='ntpServerIP' placeholder='Enter NTP Server IP' ";
    apPage += "' value='" + config_setting.ntpServerIP + "'>";
    apPage += "<label for='ntpSyncIntervalComboBox'>NTP Sync Interval:</label>";
    apPage += "<select name='ntpSyncIntervalComboBox'>";
    whatShowsInComboBox = "";
    for (int i = 30; i <= 90; i += 30) {
        if (i == 30)
        whatShowsInComboBox = "30 minutes";
        else if (i == 60)
        whatShowsInComboBox = "1 hour";
        else if (i == 90)
        whatShowsInComboBox = "1.5 hours";

        if (i == config_setting.ntpSyncIntervalComboBoxVal.toInt()) {
        apPage += "<option value='" + String(i) + "' selected>" + whatShowsInComboBox + "</option>";
        } else {
        apPage += "<option value='" + String(i) + "'>" + whatShowsInComboBox + "</option>";
        }
  }
    apPage += "</select><br>";
    apPage += "</div>";
    apPage += "<div>" + htmlMessageInfo + "</div><br>";
    apPage += "<hr>";
    apPage += "<button style='margin: 7px; background-color: LightGray;' type='submit' formaction='/loadSettings'>Load Data</button>";
    apPage += "<button style='margin: 7px; background-color: LightGray;' type='submit' formaction='/saveSettings'>Save</button>";
    apPage += "<button style='margin: 7px; background-color: LightGray;' type='submit' formaction='/restart'>Restart NodeMCU</button>";
    apPage += "</form>";
    apPage += "<script>";
    apPage += "function showPasswordFunction() {";
    apPage += "var lp = document.getElementById('login_password');";
    apPage += "var wp = document.getElementById('wifi_password');";
    apPage += "var app = document.getElementById('ap_password');";
    apPage += "if (wp.type === 'password') {";
    apPage += "  lp.type = 'text';";
    apPage += "  wp.type = 'text';";
    apPage += "  app.type = 'text';";
    apPage += "} else {";
    apPage += "  lp.type = 'password';";
    apPage += "  wp.type = 'password';";
    apPage += "  app.type = 'password';";
    apPage += "}";
    apPage += "}";
    apPage += "function toggleInput() {";
    apPage += "var inputBox = document.getElementById('Local_IP_address');";
    apPage += "var inputBox = document.getElementById('subnet_mask');";
    apPage += "var inputBox = document.getElementById('default_gateway');";
    apPage += "var checkbox = document.getElementById('toggleCheckbox');";
    apPage += "Local_IP_address.disabled = !checkbox.checked;";
    apPage += "subnet_mask.disabled = !checkbox.checked;";
    apPage += "default_gateway.disabled = !checkbox.checked;";
    apPage += "}";
    apPage += "</script>";
    apPage += "</body></html>";
    server.send(200, "text/html", apPage);
}

void handleLoginCredential() {
    String loginUsername = server.arg("login_username");
    String loginPassword = server.arg("login_password");
    if (loginUsername.equals(config_setting.loginUsername) && loginPassword.equals(config_setting.loginPassword)) {
        handleAPSettingsPage();
    } else {
        String unauthorizedString = "<div style='text-align:center; font-weight:bold; color:red;'>Unauthorized!</div>";
        server.send(401, "text/html", unauthorizedString);
    }
}
#pragma endregion
#pragma region Staion_mode_MQTT_setting_page
void ServerStationMode() {
//   blinkTicker.interval(1000);
//   blinkTicker.start();
  // server.stop();
//   delay(10);
  Serial.println("Try to connect to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");
  // Setup a web server to handle requests to save, load, ...
  server.on("/", HTTP_GET, handleWifiPage);
  server.on("/switchloginpageadmin", HTTP_GET, handleswitchLoginPage);
  server.on("/switchloginCheck", HTTP_GET, handleswitchLoginCredential);
  server.on("/switchloginpageuser", HTTP_GET, switcheruser);
  server.on("/saveUsers", HTTP_GET, handleWifiSaveUsers);
  server.on("/loadUsers", HTTP_GET, handleWifiLoadUsers);
  server.on("/startConn", HTTP_GET, handleWifiStartMQTTConnection);
  server.on("/stopConn", HTTP_GET, handleWifiStopMQTTConnection);
  server.begin();
  Serial.print("WiFi.localIP -> ");
  Serial.println(WiFi.localIP());
}


void handleWifiPage() {
  String wifiPage = "<html><head><title>Click Handler Web Server - Users Settings</title>";
  wifiPage += "<style>body { margin: 10px; }";
  wifiPage += "label, input, button { margin: 7px; font-size: 18px; }</style>";
  wifiPage += "<body>";
  wifiPage += "<h1>Click Handler Web Server - Register Users Details</h1>";
  wifiPage += "<h1 style='font-size: 20px;'>"+(Switchnumber ? Switchtext1[0] : Switchtext1[1])+" access</h1>";
  wifiPage += "<form>";
  for (int i = 0; i < NumberOfStation; i++) {
    wifiPage += "<div><label for='input" + String(i) + "'>Station </label>";
    wifiPage += "<input type='text' name='username" + String(i) + "' placeholder='None' ";
    wifiPage += "value='" + usernameText[i] + "' size='5'"+ (Switchnumber ? Switchtext2[0] : Switchtext2[1]) +">&nbsp;&nbsp;&nbsp;";
    wifiPage += "<label for='input" + String(i) + "'>Operator </label>";
    wifiPage += "<input type='text' id='description" + String(i) + "' placeholder='None' name='description" + String(i) + "'";
    wifiPage += "value='" + descriptionText[i] + "' size='5' readonly>";
    wifiPage += "<select id='operatorcombobox" + String(i) +"' onchange='myFunction"+String(i) +"()'>";
    for (int j = 0; j < operator_number; j++)
    {
      wifiPage +="<option value='"+Operators[i][j]+"'>" + Operators[i][j] + "</option>";
    }
    wifiPage += "</select>&nbsp;&nbsp;&nbsp;";
    if (!Switchnumber)
    {
      wifiPage +="<textarea style='width: 100px; height: 17px;' id='operator" +String(i)+"' name='operator" +String(i)+"'>"+operatorname[i]+"</textarea>";
      // wifiPage += "<button type='button' onclick='myFunction()'>Apply</button>";
    }
    wifiPage += "<label for='input" + String(i) + "'>Product Number </label>";
    // ----------------------------------------------------------new // ----------------------------------------------------------
    // ----------------------------------------------------------new // ----------------------------------------------------------
    wifiPage += "<input type='text' id='product_number" + String(i) + "' placeholder='None' name='product_number" + String(i) + "'";
    wifiPage += "value='" + Produt_nameText[i] + "'maxlength='11' oninput='validateNumber"+String(i)+"()' size='11'>";
    wifiPage += "<span style = 'color: red;display: none;' id='errorMessage"+String(i)+"'>Invalid number </span>";
    wifiPage += "<span style = 'color: red;display: none;' id='lengthErrorMessage"+String(i)+"'> Invalid length</span>";
    // ----------------------------------------------------------new // ----------------------------------------------------------
    // wifiPage += "<input type='text' id='product_number" + String(i) + "' placeholder='None' name='product_number" + String(i) + "'";
    // wifiPage += "value='" + Produt_nameText[i] + "' size='5'>";
    wifiPage += "</div>";
    wifiPage += "<script> function myFunction"+String(i) +"(){";
    wifiPage += "var selectBox = document.getElementById('operatorcombobox" + String(i) +"');";
    wifiPage += "var selectedText = selectBox.options[selectBox.selectedIndex].text;";
    wifiPage += "document.getElementById('description" + String(i) + "').value =selectedText; }";
    // ----------------------------------------------------------new // ----------------------------------------------------------
    // ----------------------------------------------------------new // ----------------------------------------------------------
    wifiPage += "function validateNumber"+String(i)+"() {";
    wifiPage += "var product_number" + String(i) + " = document.getElementById('product_number" + String(i) + "').value;";
    wifiPage += "var errorMessage"+String(i)+" = document.getElementById('errorMessage"+String(i)+"');";
    wifiPage += "var lengthErrorMessage"+String(i)+" = document.getElementById('lengthErrorMessage"+String(i)+"'); var validPrefixes = ['16', '26', '36', '46', '56', '66'];";
    wifiPage += "var requiredLength = 11; var isValidPrefix"+String(i)+" = validPrefixes.includes(product_number" + String(i) + ".substring(0, 2));";
    wifiPage += "var isValidLength"+String(i)+" = product_number" + String(i) + ".length === requiredLength; if (isValidPrefix"+String(i)+") {";
    wifiPage += "errorMessage"+String(i)+".style.display = 'none'; } else { errorMessage"+String(i)+".style.display = 'inline';}";
    wifiPage += "if (isValidLength"+String(i)+") { lengthErrorMessage"+String(i)+".style.display = 'none'; } else { lengthErrorMessage"+String(i)+".style.display = 'inline';}}";
    // ----------------------------------------------------------new // ----------------------------------------------------------
    wifiPage += "</script></head>";
  }
  wifiPage += "<button style='margin: 7px; background-color: LightGray;' type='submit' formaction='/saveUsers'>Save</button>";
  wifiPage += "<button style='margin: 7px; background-color: LightGray;' type='submit' formaction='/loadUsers'>Load</button>";
  wifiPage += "<button style='margin: 7px; background-color: MediumSeaGreen;' type='submit' formaction='/startConn'>Start</button>";
  wifiPage += "<button style='margin: 7px; background-color: Tomato;' type='submit' formaction='/stopConn'>Stop</button>";
  wifiPage += "</form><br><br><br>";
  wifiPage += "<div>" + htmlMessageInfo + "</div>";
  wifiPage += "<form>";
//   if (Switchnumber)
//   {
//     wifiPage += "<button style='margin: 7px; background-color: #47e0ff;' type='submit' formaction='/switchloginpageadmin'>switch to admin page</button>";
//   }else{
//     wifiPage += "<button style='margin: 7px; background-color: #47e0ff;' type='submit' formaction='/switchloginpageuser'>switch to user page</button>";  
//   }
  wifiPage += "</form></body></html>";
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.send(200, "text/html", wifiPage);
  // webService.send(200, "text/html", handlehandle);
}

void handleswitchLoginPage() {
  pagecheck = false;
  String loginPage = "<html><body>";
  loginPage += "<style> input[type='text'] { padding: 5px; margin: 5px; }  ";
  loginPage += "input[type='password'] { padding: 5px; margin: 5px; }  ";
  loginPage += "</style>";
  loginPage += "<h1 style='text-align:center;'>Click Handler Web Server - Login</h1>";
  loginPage += "<div style='text-align:center;'>";
  loginPage += "<form>";
  loginPage += "<label for='login_username_switch'>Username:</label>";
  loginPage += "<input id='login_username_switch' name='login_username_switch' minlength='3' type='text' ><br>";
  loginPage += "<label for='login_password_switch'>Password:</label>";
  loginPage += "<input id='login_password_switch' name='login_password_switch' minlength='3' type='password' ><br>";
  loginPage += "<button style='margin: 7px; background-color: LightGray;' type='submit' formaction='/switchloginCheck'>Login</button>";
  loginPage += "<button type='submit' formaction='/'>homepage</button>";
  loginPage += "</form>";
  loginPage += "</div>";
  loginPage += "</body></html>";
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.send(200, "text/html", loginPage);
}

void handleswitchLoginCredential() {
  String loginUsername = server.arg("login_username_switch");
  String loginPassword = server.arg("login_password_switch");
  String unauthorizedString = "<div style='text-align:center; font-weight:bold; color:red;'>Unauthorized!";
  unauthorizedString +="<br><a href='/switchloginpageadmin'>back to login page</a></div>";

  if (loginUsername.equals(config_setting.loginUsername) && loginPassword.equals(config_setting.loginPassword)) {
    if (!pagecheck)
    {
      Switchnumber = false;
      pagecheck = true;
      // handleWifiPage();
      server.sendHeader("Location", "/", true);
      server.send(303);
    }else{
      server.send(401, "text/html",unauthorizedString);
    }
  } else {
    server.send(401, "text/html", unauthorizedString);
  }
}

void switcheruser(){
  Switchnumber = true;
  // handleWifiPage();
  server.sendHeader("Location", "/", true);
  server.send(303);
}

// Save all usernames and descriptions in a file
void handleWifiSaveUsers() {
    if (server.args() == 0) {
        // 400: client-side error, bad syntax or cannot be fulfilled
        server.send(400, "text/plain", "Invalid request");
        return;
    }
    // usernameText[0] = {
    //     server.arg("username" + String(0))
    // };
    descriptionText[0] = {
        server.arg("description"  + String(0))
    };
    Produt_nameText[0] = {
        server.arg("product_number"  + String(0))
    };

    // user_setting.usernameText = usernameText[0];
    user_setting.descriptionText = descriptionText[0];
    user_setting.Produt_nameText = Produt_nameText[0];
    saveconfig_user(user_setting);
  
    String message = "Data saved successfully!";
    htmlMessageInfo = "<label style='font-weight:bold; color:green;'>";
    htmlMessageInfo += message;
    htmlMessageInfo += "</label>";
    htmlMessageInfo += marqueeHtml;
    htmlMessageInfo += message;
    htmlMessageInfo += marqueeEndHtml;
    Serial.println(message);
    handleWifiPage();
    server.send(200, "text/plain", message);   // 200: successfull response
}

// Fill the html with loaded usernames and descriptions
void handleWifiLoadUsers() {
    loadconfig_user(user_setting);
    // usernameText[0] = user_setting.usernameText;
    descriptionText[0] = user_setting.descriptionText;
    Produt_nameText[0] = user_setting.Produt_nameText;

    String message = "Users details loaded!";
    htmlMessageInfo = "<label style='font-weight:bold; color:green;'>";
    htmlMessageInfo += message;
    htmlMessageInfo += "</label>";
    htmlMessageInfo += marqueeHtml;
    htmlMessageInfo += message;
    htmlMessageInfo += marqueeEndHtml;
    Serial.println(message);
    handleWifiPage();
}

// Try to connect to MQTT broker
void handleWifiStartMQTTConnection() {
//   pubSubClient.setServer(mqttBrokerIPText.c_str(), mqttBrokerPortValue);
//   Serial.println("startFunction");
//   if (!pubSubClient.connected()) {
//     MQTTBrokerReconnect();
//   }
//   pubSubClient.loop();
//   String message = "MQTT Connection started!";
//   htmlMessageInfo = "<label style='font-weight:bold; color:green;'>";
//   htmlMessageInfo += message;
//   htmlMessageInfo += "</label>";
//   htmlMessageInfo += marqueeHtml;
//   htmlMessageInfo += message;
//   htmlMessageInfo += marqueeEndHtml;
//   Serial.println(message);
// pubSubClient.setServer(mqttBrokerIPText.c_str(), mqttBrokerPortValue);
//   Serial.println("startFunction");
//   if (!pubSubClient.connected()) {
//     MQTTBrokerReconnect();
//   }
//   pubSubClient.loop();
  String message = "MQTT is not implementing!";
  htmlMessageInfo = "<label style='font-weight:bold; color:green;'>";
  htmlMessageInfo += message;
  htmlMessageInfo += "</label>";
  htmlMessageInfo += marqueeHtml;
  htmlMessageInfo += message;
  htmlMessageInfo += marqueeEndHtml;
  Serial.println(message);
  handleWifiPage();
}

// void cam_show_handler(unsigned char *buffer,int length){
//   WiFiClient client = server.client();
//   String response = "HTTP/1.1 200 OK\r\n";
//   response += "Content-Type: image/jpeg\r\n";
//   client.write(response.c_str(), response.length());  // Convert String to const char* using c_str().
//   server.send_P(200, "image/jpeg",(const char*) buffer, length);
//   client.write("--frame\r\n", 8);
//   client.write("Content-Type: image/jpeg\r\n", 26);
//   String contentLength = "Content-Length: " + String(length) + "\r\n\r\n";
//   client.write(contentLength.c_str(), contentLength.length());  // Convert String to const char*
//   client.write(buffer, length);
//   client.write("\r\n", 2);
// }

// void cam_show(unsigned char *buffer,int length){
//   void camera_capture(uint8_t *ROI_buff ,const int ROI_x, const int ROI_y, const int ROI_height, const int ROI_width,const int BPP);
//   void cam_show_handler(unsigned char *buffer,int length);
//   server.on("/cam", HTTP_GET, cam_show_handler(buffer,length));
// }

// void camera_capture(){
//   camera_fb_t *fb = esp_camera_fb_get();
//     if (!fb || !fb->buf || fb->width <= 0 || fb->height <= 0) {
//         log_e("Framebuffer is null");
//         Serial.println("Framebuffer is null");
//         esp_camera_fb_return(fb);
//         // return;
//     }
//     // uint8_t *ROI_buff = (uint8_t *) malloc(ROI_x*ROI_y*BPP);
//     cropImage(fb->buf,fb->width,fb->height,ROI_x,ROI_y,ROI_height,ROI_width,BPP,ROI_buff);
//     esp_camera_fb_return(fb);
//     // return cut_buff;
// }

// Disconnect from MQTT broker
void handleWifiStopMQTTConnection() {
//   pubSubClient.disconnect();
  String message = "MQTT is not implementing!";
  htmlMessageInfo = "<label style='font-weight:bold; color:green;'>";
  htmlMessageInfo += message;
  htmlMessageInfo += "</label>";
  htmlMessageInfo += marqueeHtml;
  htmlMessageInfo += message;
  htmlMessageInfo += marqueeEndHtml;
  Serial.println(message);
  handleWifiPage();
}
#pragma endregion
#pragma region Camera_show_on_browser_setting
void camera_show_handler(){
  // camera_fb_t *fb = esp_camera_fb_get();
  // if (!fb){
  //   Serial.println("Camera capture failed");
  //   return;
  // }
  uint8_t *jpeg_buff;
  size_t jpeg_len;
  // camera_capture(ROI.ROI_buffer ,ROI.x,ROI.y , ROI.y_height, ROI.x_width,ROI.BPPN);
  // cropImage(fb->buf,fb->width,fb->height,ROI.x,ROI.y,ROI.x_width,ROI.y_height,ROI.BPPN,ROI.ROI_buffer);
  fmt2jpg(ROI.buffer,(ROI.x_width * ROI.y_height * ROI.BPPN),ROI.x_width,ROI.y_height,ROI.format,80,&jpeg_buff,&jpeg_len);
  WiFiClient client = server.client();
  String response = "HTTP/1.1 200 OK\r\n";
  response += "Content-Type: image/jpeg\r\n";
  client.write(response.c_str(), response.length());  // Convert String to const char* using c_str().
  server.send_P(200, "image/jpeg",(const char*) jpeg_buff, jpeg_len);
  client.write("--frame\r\n", 8);
  client.write("Content-Type: image/jpeg\r\n", 26);
  String contentLength = "Content-Length: " + String(jpeg_len) + "\r\n\r\n";
  client.write(contentLength.c_str(), contentLength.length());  // Convert String to const char*
  client.write(jpeg_buff, jpeg_len);
  client.write("\r\n", 2);
  // esp_camera_fb_return(fb);
  free(jpeg_buff);
}

void camera_show(){
  server.on("/cam", HTTP_GET, camera_show_handler);
}
#pragma endregion
void handleWebServerclient() {
  server.handleClient();
}
void restartNodeMCU() {
  Serial.println("Restarting NodeMCU...");
  delay(1000); // Delay before restart
  ESP.restart(); // Restart NodeMCU
}