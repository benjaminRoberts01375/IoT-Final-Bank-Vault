#pragma once
#include <PubSubClient.h>
#include <string>

namespace mqttConnection {
    
    PubSubClient MQTTClient(wifiConnection::wifiClient);
}