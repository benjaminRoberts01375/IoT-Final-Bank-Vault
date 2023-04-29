#pragma once
#include <string>
using namespace std;

// Wifi Credentials
extern const char* SSID;
extern const char* PASSWORD;

// MQTT Credentials
extern string topic;
extern const string BROKER;
extern const int PORT_CONNECTION;
extern int vaultID;

// IFTTT
extern const string iftttKey;
extern const string iftttEvent;
extern string iftttURL;
extern const int iftttPort;