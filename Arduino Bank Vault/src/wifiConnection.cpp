#include "wifiConnection.h"

WiFiClient wifiConnection::wifiClient;

void wifiConnection::connectWiFi()
{
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("Connecting to WiFi...");
        WiFi.begin(SSID, PASSWORD);
        delay(500);
    }
    Serial.println("Connected!");
}