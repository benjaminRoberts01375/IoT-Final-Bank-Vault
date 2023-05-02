#include "main.h"

struct doorStatusM {
    bool isAllowedOpen;
    bool wasOpened;
    unsigned long time;
};
/// @brief IDs of devices connected to the vault
string phoneIDs[100] = { };
/// @brief phoneID being used to setup vault
string setupPhoneID = "";
/// @brief Tracks if the vault is allowed to be opened
bool allowedOpen = false;
/// @brief Tracks if the vault is actually open
bool isOpen = false;
/// @brief Handler for ifttt connection
WiFiClient iftttClient;
/// @brief Constant for the number of door events to track
const int MAX_DOOR_TRACKER_EVENTS = 200;
/// @brief Door events
doorStatusM doorTracker[MAX_DOOR_TRACKER_EVENTS] = { };
/// Index of the last tracked event
int trackedEvents = 0;


void responseDispatcher(char *topic, uint8_t *payload, unsigned int length);

/// @brief Sends all the vault history via MQTT. 
/// This function works around the limits of what can be sent with an Arduino by
/// having each door interaction be its own MQTT message.
void sendVaultHistory() {
  for (int i = 0; i < MAX_DOOR_TRACKER_EVENTS; i++) {                        // For each door event
    if (doorTracker[i].time == 0) {                                         // If the time since arduino boot is 0
      continue;                                                               // Skip it
    }

    StaticJsonDocument<200> doc;                                            // Create the JSON doc
    JsonObject vault = doc.createNestedObject("vault");                     // Create the vault object within the JSON doc
    vault["id"] = vaultID;                                                  // Set the id of the vault
    JsonObject doorStatus = doc.createNestedObject("doorStatus");           // Create the door status object
    doorStatus["isAllowedOpen"] = doorTracker[i].isAllowedOpen;             // Set if the door was allowed to be opened
    doorStatus["wasOpened"] = doorTracker[i].wasOpened;                     // Set if the door was opened
    doorStatus["time"] = doorTracker[i].time;                               // Set the time of the occurance
    string jsonString;                                                      // String for holding JSON
    serializeJson(doc, jsonString);                                         // Serialize json to a string
    mqttConnection::MQTTClient.publish(topic.c_str(), jsonString.c_str());  // Publish
  }
}

/// @brief Adds and overwrites the door tracker values
void trackDoorStatus() {
  doorTracker[trackedEvents] = { allowedOpen, isOpen, millis() }; // At the trackedEvents index, create a new struct to capture the current status
  trackedEvents = (trackedEvents + 1) % MAX_DOOR_TRACKER_EVENTS;  // Increment the tracked events
  sendVaultHistory();                                             // Send the vault history now that it has updated
}

/// @brief Sets up vault based on phone request
/// @param topic MQTT topic
/// @param payload MQTT response
/// @param length Length of MQTT response
void configureVault(char *topic, uint8_t *payload, unsigned int length) {
  StaticJsonDocument<200> JSONResponse;
  deserializeJson(JSONResponse, payload);
  if (mqttConnection::jsonCheck(JSONResponse, setupPhoneID)) {  // If valid JSON...
    Serial.println("Valid JSON");
    string phoneID = JSONResponse["phoneID"];                     // Save the phone ID
    int index = 0;
    for (int i = 0; i < sizeof(phoneIDs); i++) {                  // For each index in the phoneIDs array...
      if (phoneIDs[index] == JSONResponse["phoneID"]) {             // Check for the existing entry
        return;                                                       // Return
      }
      else if (phoneIDs[index] != "") {                             // If there's NOT an empty entry in the phoneIDs...
        index += 1;                                                   // Keep searching...
      }
      else {                                                        // An empty index was found...
        break;                                                        // That's where the new phoneID should go
      }
    }
    phoneIDs[index] = phoneID;                                    // Add the phone ID to the array
    Serial.println("Added phone id to list");
    setupPhoneID = "";                                            // Reset the stored phoneID
    mqttConnection::MQTTClient.setCallback(responseDispatcher);   // Reset the callback function
  }
}

/// @brief Call to IFTTT for vault being broken into
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
  StaticJsonDocument<200> JSONResponse;                                       // JSON document
  deserializeJson(JSONResponse, payload);                                     // Deserialize payload into JSON
  if (mqttConnection::jsonCheck(JSONResponse, "", false)) {                   // If it's valid JSON for this vault...
    if (JSONResponse.containsKey("requestType")) {                              // Check if there's a request type
      if (JSONResponse["requestType"] == mqttConnection::checkSetup) {            // Check if the request type is "check setup"
        displaySetupStatus();                                                       // Display the current setup status
      }
      else if (JSONResponse["requestType"] == mqttConnection::requestSetup && // Check if the request type is "request setup" and
      mqttConnection::jsonCheck(JSONResponse, "") &&                          // if the JSON is valid JSON for this vault and
      JSONResponse.containsKey("phoneID")) {                                  // if there's a phone id
        enterSetup(JSONResponse["phoneID"]);                                    // Then enter setup
      }
    }
  }
}

/// @brief Basic function to kickoff setup by setting the confirmSetup to be the callback
void beginVaultSetup() {
  displaySetupStatus();                                     // Announce the vault needs setup
  mqttConnection::MQTTClient.setCallback(vaultCheckSetup);  // Set the MQTT callback function to vaultCheckSetup
}

/// @brief A generic handler for MQTT messages after the vault is setup
/// @param topic MQTT topic
/// @param payload MQTT message
/// @param length Length of the MQTT message
void responseDispatcher(char *topic, uint8_t *payload, unsigned int length) {
  StaticJsonDocument<200> doc;                        // JSON doc
  deserializeJson(doc, payload);                      // Deserialize JSON into the document
  if (mqttConnection::jsonCheck(doc, "")) {           // Check if the JSON is meant for this vault
    bool validPhone = false;
    for (int i = 0; i < sizeof(phoneIDs); i++) {      // For each phoneID...
      if (phoneIDs[i] == doc["phoneID"]) {              // If the delivered phoneID matches a stored phone ID
        validPhone = true;                                // Track it being a valid phone
        break;                                            // and exit the loop
      }
    }
    if (validPhone) {                                 // If it's a valid phone
      if (doc["interaction"] == "setup") {              // If we're told to enter setup...
        beginVaultSetup();                                // Enter setup
      }
      else if (doc["interaction"] == "remove") {        // If we're told to remove the phoneID...
        for (int i = 0; i < sizeof(phoneIDs); i++) {      // For each phoneID
          if (phoneIDs[i] == doc["phoneID"]) {              // If the delivered phoneID matches a given phoneID
            phoneIDs[i] = "";                                 // Remove that phoneID
            break;                                            // and exit the loop
          }
        }
      }
      else if (doc["interaction"] == "openVault") {   // If we're told to open the vault...
        allowedOpen = true;                             // Set allowedOpen to true
        doorServo.write(180);                           // Move the servo into the open position
        digitalWrite(LED_R_PIN, LOW);                   // Turn off the red LED
        digitalWrite(LED_G_PIN, HIGH);                  // Turn on the green LED
        digitalWrite(LED_B_PIN, LOW);                   // Turn off the blue LED
      }
      else if (doc["interaction"] == "closeVault") {  // If we're told to close the vault...
        allowedOpen = false;                            // Set allowedOpen to false
        doorServo.write(0);                             // Move the servo into the closed position
        digitalWrite(LED_R_PIN, HIGH);                  // Turn on the red LED
        digitalWrite(LED_G_PIN, LOW);                   // Turn off the green LED
        digitalWrite(LED_B_PIN, LOW);                   // Turn off the blue LED
      }
    }
  }
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
  byte buttonState = digitalRead(DOOR_PIN);           // Read the current status of the vault
  mqttConnection::reconnectMQTTClient();              // Ensure MQTT connection
  mqttConnection::MQTTClient.loop();                  // Read MQTT


  if (buttonState == HIGH) {                          // If the vault is actually open
    doorServo.write(0);
    if (!isOpen) {                                      // If the vault is tracked to be closed
      if (!allowedOpen) {                               // If the vault is tracked to not be allowed to be open
        Serial.println("Vault has been broken into");     // Then the vault has been broken into
        digitalWrite(LED_R_PIN, HIGH);                    // Turn on the red LED
        digitalWrite(LED_G_PIN, LOW);                     // Turn off the green LED
        digitalWrite(LED_B_PIN, LOW);                     // Turn off the blue LED
        callWebhook();                                    // Alert users
      }
      else {                                            // If the vault is allowed to be open...
        digitalWrite(LED_R_PIN, LOW);                     // Turn off the red LED
        digitalWrite(LED_G_PIN, HIGH);                    // Turn on the green LED
        digitalWrite(LED_B_PIN, LOW);                     // Turn off the blue LED
      }
      isOpen = true;                                    // Track the vault being open
      trackDoorStatus();                                // Track the current status of the vault
    }
  }
  else {                                              // If the vault is actually closed
    if (isOpen) {                                       // If the vault was tracked to be open
      isOpen = false;                                     // Track the vault being closed
      trackDoorStatus();                                  // Track the current status of the vault
    }
  }
}