#include "data_transfer_management.h"
extern String usernameText[7];// = { "1", "2", "3", "4", "5", "6", "7" };
// Define vectors to store log entries
std::vector<LogEntry> logEntriesForDB,newlogEntriesForDB;//, logEntriesForBroker;

extern USER_SETTING user_setting;
extern CONFIG_SETTING config_setting;

// uint32_t previousTimeToSendToDB = millis();
unsigned long previousTimeToSendToDB = millis();
unsigned long sendToDBTimeInterval = config_setting.timeIntervalToSendToDBComboBoxVal.toInt();// * 60 * 1000;
// uint32_t sendToDBTimeInterval = 15 * 60 * 1000;
// unsigned long sendToDBTimeInterval = 15 * 60 * 1000;


void saveLogEntriesInFile(){//String fileName) {
  // Check if the file not exists, create it
//   File file = LittleFS.open(fileName, "w");
//   if (!file) {
//     Serial.print("Failed to open file, ");
//     Serial.println(fileName);
//     return;
//   }
//   file.close();
  // Open the file in append mode
//   file = LittleFS.open(fileName, "a");
//   if (!file) {
//     Serial.print("Failed to open file for appending, ");
//     Serial.println(fileName);
//     return;
//   }
    // StaticJsonDocument<1024> doc;
    JsonDocument(doc);
//   if (fileName == clickHandlerLogEntriesForDBFile)  {
    for (int i = 0; i < logEntriesForDB.size(); i++) {
      doc["username"] = logEntriesForDB[i].username;
      doc["user_description"] = logEntriesForDB[i].user_description;
      doc["log_date"] = logEntriesForDB[i].log_date;
      doc["log_time"] = logEntriesForDB[i].log_time;
      doc["product_number_value"] = logEntriesForDB[i].product_number_value;
      doc["press_result"] = logEntriesForDB[i].press_result;

    //   serializeJson(doc, file);
    //   file.println();
    }
    logEntriesForDB.clear();
//   }
//   if (fileName == clickHandlerlogEntriesForBrokerFile)  {
//     for (int i = 0; i < logEntriesForBroker.size(); i++) {
//       doc["username"] = logEntriesForBroker[i].username;
//       doc["user_description"] = logEntriesForBroker[i].user_description;
//       doc["log_date"] = logEntriesForBroker[i].log_date;
//       doc["log_time"] = logEntriesForBroker[i].log_time;
//       doc["product_number_value"] = logEntriesForBroker[i].product_number_value;
//       doc["press_result"] = logEntriesForBroker[i].press_result;

//       serializeJson(doc, file);
//       file.println();
//     }
//     logEntriesForBroker.clear();
//   }
//   file.close();
  Serial.println("saveLogEntriesInFile done ");
}


void sendBatchedLogsToDB() {
    Serial.print("sendBatchedLogsToDB, ");
    Serial.println(logEntriesForDB.size());
    if (logEntriesForDB.size() == 0) {
        return;
    }
    // Check WiFi connection status
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi Disconnected");
        saveLogEntriesInFile();//clickHandlerLogEntriesForDBFile);
        return;
    }
    HTTPClient http;
    WiFiClient client;
    // Your Domain name with URL path or IP address with path
    if (!http.begin(client, config_setting.phpServerLink)) {
        Serial.print("Error in HTTPClient - failed to connect, or web service link is not correct, ");
        Serial.println(config_setting.phpServerLink);
        saveLogEntriesInFile();//clickHandlerLogEntriesForDBFile);
        return;
    }
    // Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    // Create a new vector to store elements of logEntriesForDB that are not sent

    String httpRequestData, dataPrefixStrI, dataStr = "&data";
    // Loop to process entries in batches
    for (int j = 0; j < logEntriesForDB.size(); j += config_setting.batchSizeToSendToDBComboBoxVal.toInt()) {
        httpRequestData = "api_key=" + config_setting.phpApiKeyValue;

        // Prepare HTTP POST request data for the current batch of entries
        for (int i = j; i < j + config_setting.batchSizeToSendToDBComboBoxVal.toInt() && i < logEntriesForDB.size(); i++) {
        const LogEntry& entry = logEntriesForDB[i];
        dataPrefixStrI = dataStr + i;

        httpRequestData += dataPrefixStrI + "username=" + entry.username +
                            dataPrefixStrI + "user_description=" + entry.user_description +
                            dataPrefixStrI + "log_date=" + entry.log_date +
                            dataPrefixStrI + "log_time=" + entry.log_time +
                            dataPrefixStrI + "product_number_value=" + entry.product_number_value +
                            dataPrefixStrI + "press_result=" + entry.press_result
                            ;
        }

        Serial.print("httpRequestData: ");
        Serial.println(httpRequestData);
        int httpResponseCode = http.POST(httpRequestData);
        // HTTP Response Code 200:OK, 404:Not Found, 400:Bad Request, 500: Internal Server Error
        // HTTP Response Code -1:Net Connectivity Issue

        if (httpResponseCode != 200) {
        Serial.print("=> HTTP Response Error code: ");
        Serial.println(httpResponseCode);
        // Add the processed entries to list for removal
        for (int i = j; i < j + config_setting.batchSizeToSendToDBComboBoxVal.toInt() && i < logEntriesForDB.size(); i++) {
            newlogEntriesForDB.push_back(logEntriesForDB[i]);
        }
        }
    }
    // Free resources
    http.end();

    // Update logEntriesForDB with newlogEntriesForDB
    logEntriesForDB = newlogEntriesForDB;
}


void sendLogToDB(LogEntry newEntry) {   
    // Add the log entry to the buffer
    logEntriesForDB.push_back(newEntry);

    // Check if the buffer size has reached a threshold or time threshold
    // Here you can set a condition to send data after a certain number of entries or a time interval.
    if ((logEntriesForDB.size() >= config_setting.batchSizeToSendToDBComboBoxVal.toInt()) ||
        (millis() - previousTimeToSendToDB >= (config_setting.timeIntervalToSendToDBComboBoxVal.toInt()*60*1000))) {
        sendBatchedLogsToDB();
        previousTimeToSendToDB = millis();
    }
}

// void publishClickEventMessage(int pinNo) {
void publishClickEventMessage(String press_result_value) {
    Serial.println("Start publish");
    String currentTime;
    String currentDate;
    get_time_date(currentTime,currentDate);
    // String currentDate = getCurrentDateFromLocalVariables();
    // String currentTime = getCurrentTimeFromLocalVariables();
    //   String username = removeNewline(usernameText[pinNo - 1]);
    String username = removeNewline(usernameText[0]);
    //   String description = removeNewline(descriptionText[pinNo - 1]);
    String description = removeNewline(user_setting.descriptionText);
    //   String product_number = removeNewline(Produt_nameText[pinNo - 1]);
    String product_number = removeNewline(user_setting.Produt_nameText);
    String press_result = press_result_value;
    LogEntry newEntry = {username, description, currentDate, currentTime,product_number,press_result};
    sendLogToDB(newEntry);
//     switch (publishToDBOrBrokerComboBoxValue) {
//     case PUBLISH_TO_DB:
//         sendLogToDB(newEntry);
//         break;
//     case PUBLISH_TO_BROKER:
//         sendLogToBroker(newEntry);
//         break;
//     case PUBLISH_TO_DB_AND_BROKER:
//         sendLogToDB(newEntry);
//         sendLogToBroker(newEntry);
//         break;
//   }
    Serial.println("stop publish");
}

//for mqtt
// void sendLogToBroker(LogEntry newEntry) {
//   if (!pubSubClient.connected()) {
//     MQTTBrokerReconnect();
//     // Add the log entry to the buffer
//     logEntriesForBroker.push_back(newEntry);
//     return;
//   }

//   String mqttMessage = newEntry.username
//                        + ", " + newEntry.user_description
//                        + ", " + newEntry.log_date
//                        + ", " + newEntry.log_time
//                        + ", " + newEntry.product_number_value
//                        + ", " + newEntry.press_result
//                        ;
//   Serial.println("MQTT message is ==> " + mqttMessage);

//   pubSubClient.publish(mqttClickEventTopicText.c_str(), mqttMessage.c_str());
//   //    delay(500);
// }

// // Custom function to check if an entry is present in entriesToRemove
// bool isEntryInToBeRemoved(const LogEntry& entry, const std::vector<LogEntry>& logEntriesForDBToBeRemoved) {
//   for (const auto& logEntry : logEntriesForDBToBeRemoved) {
//     if (   (logEntry.username == entry.username)
//         && (logEntry.user_description == entry.user_description)
//         && (logEntry.log_date == entry.log_date)
//         && (logEntry.log_time == entry.log_time)
//         && (logEntry.product_number_value == entry.product_number_value)
//         && (logEntry.press_result == entry.press_result))
//         {
//       return true;
//     }
//   }
//   return false;
// }