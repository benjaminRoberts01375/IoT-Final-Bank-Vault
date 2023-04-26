#include "main.h"

int loops = 1;
int vaultID = 0;
string phoneIDs[100] = { };

void configureVault(char *topic, uint8_t *payload, unsigned int length) {
  Serial.println("Response for configuring vault");
  StaticJsonDocument<200> JSONResponse;
  DeserializationError error = deserializeJson(JSONResponse, payload);

  if (error) {
    Serial.println("Not able to serialize the confirmation JSON.");
    return;
  }

  if (JSONResponse.containsKey("phoneID")) {
    Serial.println("Valid JSON");
    string phoneID = JSONResponse["phoneID"];
    int index = 0;
    for (int i = 0; i < sizeof(phoneIDs); i++) {
      if (phoneIDs[index] == JSONResponse["phoneID"]) {  // Prevent duplicate entry
        return;
      }
      else if (phoneIDs[index] != "") {           // Keep searching until an empty spot is found
        index += 1;
      }
      else {                                      // We found a spot, stop looking
        break;
      }
    }
    phoneIDs[index] = phoneID;
    Serial.println("Added phone id to list");
    mqttConnection::MQTTClient.setCallback(mqttConnection::clientCallback);
  }
  else {
    Serial.println("Invalid JSON response");
  }
}

void enterSetup(int phoneID) {
  Serial.println("Entering setup.");                                      // Print status
  StaticJsonDocument<200> doc;                                            // Create a json doc
  doc["phoneID"] = phoneID;                                               // ID of the phone
  doc["confirmSetup"] = true;                                             // Inform the mobile device that setup is a go
  JsonObject vaultInfo = doc.createNestedObject("vaultM");                // Create vault info 
  vaultInfo["id"] = vaultID;                                              // Add the ID of the vault
  string jsonString;                                                      // Storage for JSON object as a string
  serializeJson(doc, jsonString);                                         // Serialize json to a string
  mqttConnection::MQTTClient.publish(TOPIC.c_str(), jsonString.c_str());  // Publish to the topic
  mqttConnection::MQTTClient.setCallback(configureVault);                 // Set the callback to the next stage of setup
}

void displaySetupStatus(int phoneID) {
  Serial.println("Displaying setup status.");
  StaticJsonDocument<200> doc;
  JsonObject vaultInfo = doc.createNestedObject("vaultM");                // Create vault info 
  vaultInfo["id"] = vaultID;                                              // Add the ID of the vault
  doc["phoneID"] = phoneID;
  doc["setupResponse"] = "Needs setup";
  string jsonString;
  serializeJson(doc, jsonString);
  mqttConnection::MQTTClient.publish(TOPIC.c_str(), jsonString.c_str());
}

void confirmSetup(char *topic, uint8_t *payload, unsigned int length) {
  Serial.println("Got a response");
  StaticJsonDocument<200> JSONResponse;
  DeserializationError error = deserializeJson(JSONResponse, payload);

  if (error) {
    Serial.println("Not able to serialize the confirmation JSON.");
    return;
  }

  if (JSONResponse.containsKey("phoneID") && JSONResponse.containsKey("requestType")) {       // If the request has a "request type" field
    int phoneID = JSONResponse["phoneID"];
    string requestType = JSONResponse["requestType"];
    Serial.print("Request type is: ");
    Serial.println(requestType.c_str());
    if (requestType == mqttConnection::requestSetup && JSONResponse.containsKey("vaultM")) {    // If a vault object exists
      JsonObject nestedObj = JSONResponse["vaultM"].as<JsonObject>();
      if (nestedObj.containsKey("vaultID") && nestedObj["vaultID"] == vaultID) {                  // If a vault ID is specified
        enterSetup(phoneID);
      }
      else {
        Serial.println("No ID specified, or ID was incorrect");
      }
    }
    else if (requestType == mqttConnection::checkSetup) {
      displaySetupStatus(phoneID);
    }
    Serial.println("Unknown request");
  }
  else {
    Serial.println("Invalid JSON response");
  }
}

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