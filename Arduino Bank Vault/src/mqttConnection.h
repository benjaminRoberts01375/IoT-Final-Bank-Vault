#pragma once
#include <PubSubClient.h>
#include <credentials.h>
#include <string>
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

    const string requestSetup = "request setup";
    const string checkSetup = "check setup";
}