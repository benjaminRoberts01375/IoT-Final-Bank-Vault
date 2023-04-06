#pragma once
#include <PubSubClient.h>
#include <credentials.h>
#include <string>
#include "wifiConnection.h"

namespace mqttConnection
{
    /// @brief Connect to MQTT client if not already connected (ex. startup or random disconnect)
    void reconnectMQTTClient();
    PubSubClient MQTTClient(wifiConnection::wifiClient);
}