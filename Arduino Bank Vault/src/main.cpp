#include "main.h"

int loops = 1;
int vaultID = 0;
string phoneIDs[100] = { };

/// @brief Sets up vault based on phone request
/// @param topic MQTT topic
/// @param payload MQTT response
/// @param length Length of MQTT response
void configureVault(char *topic, uint8_t *payload, unsigned int length) {
  Serial.println("Response for configuring vault");
  StaticJsonDocument<200> JSONResponse;                                 // MQTT document
  DeserializationError error = deserializeJson(JSONResponse, payload);  // Deserialize MQTT response to JSON

  if (error) {                                                          // Check for errors
    Serial.println("Not able to serialize the confirmation JSON.");
    return;
  }

  if (JSONResponse.containsKey("phoneID")) {                                  // If there's a phone ID
    Serial.println("Valid JSON");     
    string phoneID = JSONResponse["phoneID"];                                   // Save the phone ID
    int index = 0;      
    for (int i = 0; i < sizeof(phoneIDs); i++) {                              // For each index in the phoneIDs array...
      if (phoneIDs[index] == JSONResponse["phoneID"]) {                         // Check for the existing entry
        return;                                                                   // Return
      }     
      else if (phoneIDs[index] != "") {                                         // If there's NOT an empty entry in the phoneIDs...
        index += 1;                                                               // Keep searching...
      }     
      else {                                                                    // An empty index was found
        break;      
      }     
    }     
    phoneIDs[index] = phoneID;                                                // Add the phone ID to the array
    Serial.println("Added phone id to list");
    mqttConnection::MQTTClient.setCallback(mqttConnection::clientCallback);   // Reset the callback function
  }
  else {
    Serial.println("Invalid JSON response");
  }
}

/// @brief Let the phone know it's ok to setup, and that it's beginning
/// @param phoneID Phone that sent the request
void enterSetup(int phoneID) {
  Serial.println("Entering setup.");                                      // Print status
  StaticJsonDocument<200> doc;                                            // Create a json doc
  doc["phoneID"] = phoneID;                                               // ID of the phone
  doc["confirmSetup"] = true;                                             // Inform the mobile device that setup is a go
  JsonObject vaultInfo = doc.createNestedObject("vault");                 // Create vault info 
  vaultInfo["id"] = vaultID;                                              // Add the ID of the vault
  string jsonString;                                                      // Storage for JSON object as a string
  serializeJson(doc, jsonString);                                         // Serialize json to a string
  mqttConnection::MQTTClient.publish(topic.c_str(), jsonString.c_str());  // Publish to the topic
  mqttConnection::MQTTClient.setCallback(configureVault);                 // Set the callback to the next stage of setup
}

/// @brief Broadcasts if this vault needs to be setup
/// @param phoneID Phone ID that sent the request
void displaySetupStatus(int phoneID) {
  Serial.println("Displaying setup status.");
  StaticJsonDocument<200> doc;                                            // JSON document
  JsonObject vaultInfo = doc.createNestedObject("vault");                 // Create vault info 
  vaultInfo["id"] = vaultID;                                              // Add the ID of the vault
  doc["phoneID"] = phoneID;                                               // Set the phone ID
  doc["setupResponse"] = "Needs setup";                                   // Set the response
  string jsonString;
  serializeJson(doc, jsonString);                                         // Serialize JSON to a string
  mqttConnection::MQTTClient.publish(topic.c_str(), jsonString.c_str());  // Publish
}

/// @brief Handles checking if the device should be setup, or could be setup
/// @param topic Topic from MQTT
/// @param payload MQTT response
/// @param length Length of MQTT response
void confirmSetup(char *topic, uint8_t *payload, unsigned int length) {
  Serial.println("Got a response");
  StaticJsonDocument<200> JSONResponse;                                                       // JSON document
  DeserializationError error = deserializeJson(JSONResponse, payload);                        // Convert MQTT response to JSON

  if (error) {                                                                                // Check if MQTT response was JSON
    Serial.println("Not able to serialize the confirmation JSON.");
    return;
  }

  if (JSONResponse.containsKey("phoneID") && JSONResponse.containsKey("requestType")) {       // If the response has a "request type" and phoneID field
    int phoneID = JSONResponse["phoneID"];                                                    // Save the ID of the phone
    string requestType = JSONResponse["requestType"];                                         // Save the request type
    Serial.print("Request type is: ");
    Serial.println(requestType.c_str());
    if (requestType == mqttConnection::requestSetup && JSONResponse.containsKey("vault")) {     // If a vault object exists
      JsonObject nestedObj = JSONResponse["vault"].as<JsonObject>();                              // Save the vault object
      if (nestedObj.containsKey("vaultID") && nestedObj["vaultID"] == vaultID) {                  // If a vault ID is specified
        enterSetup(phoneID);                                                                        // Enter setup for this vault
      }
      else {
        Serial.println("No ID specified, or ID was incorrect");
      }
    }
    else if (requestType == mqttConnection::checkSetup) {                                     // If the request type was asking if this vault needed setting up
      displaySetupStatus(phoneID);                                                              // Display response
    }
    Serial.println("Unknown request");
  }
  else {
    Serial.println("Invalid JSON response");
  }
}

/// @brief Basic function to kickoff setup by setting the confirmSetup to be the callback
void setupVault() {
  mqttConnection::MQTTClient.setCallback(confirmSetup);
}

void setup() {
  Serial.begin(9600);
  while (!Serial);                        // Wait for Serial to be ready
  wifiConnection::connectWiFi();          // Connect to wifi
  mqttConnection::configureMQTTClient();  // Setup MQTT
  
  pinMode(LED_R_PIN, OUTPUT);             // Red LED pin
  pinMode(LED_G_PIN, OUTPUT);             // Green LED pin
  pinMode(LED_B_PIN, OUTPUT);             // Blue LED pin
  doorServo.attach(SERVO_PIN);            // Servo pin
  pinMode(DOOR_PIN, INPUT_PULLUP);        // Button pin
  
  doorServo.write(0);                     // Move servo to starting position
  
  randomSeed(analogRead(0));              // Setup the random seed
  vaultID = random(50001);              // Choose a random number from 0 - 50,000
  
  digitalWrite(LED_B_PIN, HIGH);          // Set LED to blue
  
  setupVault();                           // Enter setup mode
}

void loop() {
  byte buttonState = digitalRead(DOOR_PIN);
  mqttConnection::reconnectMQTTClient();
  mqttConnection::MQTTClient.loop();

  if (buttonState == LOW) {
      doorServo.write(180);
  }
  else {
      doorServo.write(0);
  }

  // if (millis() / (5000 * loops) >= 1.0) {
  //   loops += 1;
  //   Serial.println("Sending...");
  //   mqttConnection::MQTTClient.publish(TOPIC.c_str(), "Test");
  // }
}