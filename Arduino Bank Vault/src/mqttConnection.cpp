#include "mqttConnection.h"

void mqttConnection::reconnectMQTTClient() {
  while (!MQTTClient.connected())
  {
    Serial.print("Attempting MQTT connection... ");
    if (MQTTClient.connect(CLIENT_NAME.c_str()))
    {
      Serial.println("Connected to broker.");
      MQTTClient.subscribe(TOPIC.c_str()); // TO SUBCRIBE A TOPIC
    }
    else
    {
      Serial.print("Retying in 5 seconds - failed, rc=");
      Serial.println(MQTTClient.state());

      delay(5000);
    }
  }
}