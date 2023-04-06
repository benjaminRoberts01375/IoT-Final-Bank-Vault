#pragma once
#include <PubSubClient.h>
#include <credentials.h>
#include <string>
#include "wifiConnection.h"

namespace mqttConnection
{
    /// @brief Setup an MQTT instance
    /// @param WiFiClient Wifi to use to connect to MQTT broker
    PubSubClient MQTTClient(wifiConnection::wifiClient);
    
    /// @brief Connect to MQTT client if not already connected (ex. startup or random disconnect)
    void reconnectMQTTClient();

    /// @brief Read the MQTT buffer to determine received message.
    /// @param topic MQTT Topic
    /// @param payload Received data from MQTT
    /// @param length Length of the data received
    /// @return The interpreted message
    String clientCallback(char *topic, uint8_t *payload, unsigned int length);
}