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

String mqttConnection::clientCallback(char *topic, uint8_t *payload, unsigned int length) {
    char buff[length + 1];
    for (int i = 0; i < length; i++)
    {
        buff[i] = (char)payload[i];
    }
    buff[length] = '\0';

    Serial.print("Message received - ");
    Serial.println(buff);
    String message = buff;
    return message;
}