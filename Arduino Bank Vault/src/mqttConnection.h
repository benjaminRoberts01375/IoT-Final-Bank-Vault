#pragma once
#include <PubSubClient.h>
#include <credentials.h>
#include <string>
#include <ArduinoJson.h>
#include "wifiConnection.h"

namespace mqttConnection
{
    /// @brief Setup an MQTT instance
    /// @param WiFiClient Wifi to use to connect to MQTT broker
    extern PubSubClient MQTTClient;
    
    /// @brief Sets the MQTTClient to use the server and port
    void configureMQTTClient();

    /// @brief Connect to MQTT client if not already connected (ex. startup or random disconnect)
    void reconnectMQTTClient();

    /// @brief Read the MQTT buffer to determine received message.
    /// @param topic MQTT Topic
    /// @param payload Received data from MQTT
    /// @param length Length of the data received
    /// @return The interpreted message
    void clientCallback(char *topic, uint8_t *payload, unsigned int length);

    /// @brief Checks a payload to see if it's meant for this Arduino
    /// @param payload Payload from MQTT
    /// @param phoneID Phone ID to check
    /// @return A boolean for if the JSON is valid
    bool jsonCheck(uint8_t *payload, string phoneID);

    const string requestSetup = "request setup";
    const string checkSetup = "check setup";
}