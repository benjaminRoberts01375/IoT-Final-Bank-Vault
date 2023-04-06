#pragma once
#include <WiFi.h>
#include "credentials.h"

namespace wifiConnection
{
    /// @brief Handles connecting to a wifi network if not already connected
    void connectWiFi();
    
    extern WiFiClient wifiClient;
}