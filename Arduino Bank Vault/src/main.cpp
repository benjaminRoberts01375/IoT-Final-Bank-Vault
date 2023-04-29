#include "main.h"

struct doorStatusM {
    bool isAllowedOpen;
    bool wasOpened;
    unsigned long time;
};

int loops = 1;
string phoneIDs[100] = { };
string setupPhoneID = "";
bool allowedOpen = false;
bool isOpen = false;

WiFiClient iftttClient;

const int maxDoorTrackerEvents = 200;
doorStatusM doorTracker[maxDoorTrackerEvents] = { };
int trackedEvents = 0;

/// @brief Adds and overwrites the door tracker values
void trackDoorStatus() {
  doorTracker[trackedEvents] = { allowedOpen, isOpen, millis() };
  trackedEvents = (trackedEvents + 1) % maxDoorTrackerEvents;
}

/// @brief Sets up vault based on phone request
/// @param topic MQTT topic
/// @param payload MQTT response
/// @param length Length of MQTT response
void configureVault(char *topic, uint8_t *payload, unsigned int length) {
  StaticJsonDocument<200> JSONResponse;
  deserializeJson(JSONResponse, payload);
  if (mqttConnection::jsonCheck(JSONResponse, setupPhoneID)) {
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
    setupPhoneID = "";                                                        // Reset the stored phoneID
    mqttConnection::MQTTClient.setCallback(mqttConnection::clientCallback);   // Reset the callback function
  }
}

void callWebhook() {
  StaticJsonDocument<200> doc;
  doc["value1"] = "Hello from Arduino land!";
  doc["value2"] = iftttEvent;
  doc["value3"] = vaultID;
  iftttClient.connect(iftttURL.c_str(), iftttPort);
  string json;
  serializeJson(doc, json);
  String requestInfo = "";
  requestInfo.concat("POST /trigger/");
  requestInfo.concat(iftttEvent.c_str());
  requestInfo.concat("/with/key/");
  requestInfo.concat(iftttKey.c_str());
  requestInfo.concat(" HTTP/1.1\r\n");
  requestInfo.concat("Host: ");
  requestInfo.concat(iftttURL.c_str());
  requestInfo.concat("\r\n");
  requestInfo.concat("Content-Type: application/json\r\n");
  requestInfo.concat("Content-Length: ");
  requestInfo.concat(json.length());
  requestInfo.concat("\r\n");
  requestInfo.concat("\r\n");
  requestInfo.concat(json.c_str());
  Serial.println(requestInfo.c_str());
  iftttClient.print(requestInfo.c_str());
  iftttClient.stop();
}

/// @brief Let the phone know it's ok to setup, and that it's beginning
/// @param phoneID Phone that sent the request
void enterSetup(string phoneID) {
  Serial.println("Entering setup.");                                      // Print status
  setupPhoneID = phoneID;                                                 // Cache the phone ID to prevent incorrect phone trying to connect
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
void displaySetupStatus() {
  Serial.println("Displaying setup status.");
  StaticJsonDocument<200> doc;                                            // JSON document
  JsonObject vaultInfo = doc.createNestedObject("vault");                 // Create vault info 
  vaultInfo["id"] = vaultID;                                              // Add the ID of the vault
  doc["setupResponse"] = "Needs setup";                                   // Set the response
  string jsonString;
  serializeJson(doc, jsonString);                                         // Serialize JSON to a string
  mqttConnection::MQTTClient.publish(topic.c_str(), jsonString.c_str());  // Publish
}

/// @brief Handles checking if the device should be setup, or could be setup
/// @param topic Topic from MQTT
/// @param payload MQTT response
/// @param length Length of MQTT response
void vaultCheckSetup(char *topic, uint8_t *payload, unsigned int length) {
  StaticJsonDocument<200> JSONResponse;                                                       // JSON document
  deserializeJson(JSONResponse, payload);
  if (mqttConnection::jsonCheck(JSONResponse, "", false)) {  
    if (JSONResponse.containsKey("requestType")) {
      if (JSONResponse["requestType"] == mqttConnection::checkSetup) {
        displaySetupStatus();
      }
      else if (JSONResponse["requestType"] == mqttConnection::requestSetup && 
      mqttConnection::jsonCheck(JSONResponse, "") &&
      JSONResponse.containsKey("phoneID")) {
        enterSetup(JSONResponse["phoneID"]);
      }
    }
  }
}

/// @brief Basic function to kickoff setup by setting the confirmSetup to be the callback
void beginVaultSetup() {
  displaySetupStatus();
  mqttConnection::MQTTClient.setCallback(vaultCheckSetup);
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
  vaultID = random(50001);                // Choose a random number from 0 - 50,000
  
  digitalWrite(LED_B_PIN, HIGH);          // Set LED to blue
  
  beginVaultSetup();                      // Enter setup mode
}

void loop() {
  byte buttonState = digitalRead(DOOR_PIN);
  mqttConnection::reconnectMQTTClient();
  mqttConnection::MQTTClient.loop();

  if (buttonState == LOW) {
    if (isOpen) {
        doorServo.write(180);
        isOpen = false;
        digitalWrite(LED_R_PIN, HIGH);
        digitalWrite(LED_G_PIN, HIGH);
        digitalWrite(LED_B_PIN, LOW);
        trackDoorStatus();
    }
  }
  else {
      doorServo.write(0);
      if (!isOpen) {
        if (!allowedOpen) {
          Serial.println("Vault has been broken into");
          digitalWrite(LED_R_PIN, HIGH);
          digitalWrite(LED_G_PIN, LOW);
          digitalWrite(LED_B_PIN, LOW);
          callWebhook();
        }
        else if (allowedOpen) {
          digitalWrite(LED_R_PIN, LOW);
          digitalWrite(LED_G_PIN, HIGH);
          digitalWrite(LED_B_PIN, LOW);
        }
        isOpen = true;
        trackDoorStatus();
      }
  }
  // if (millis() / (5000 * loops) >= 1.0) {
  //   loops += 1;
  //   Serial.println("Sending...");
  //   mqttConnection::MQTTClient.publish(TOPIC.c_str(), "Test");
  // }
}