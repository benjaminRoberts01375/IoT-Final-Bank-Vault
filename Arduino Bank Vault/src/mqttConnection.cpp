#include "mqttConnection.h"

PubSubClient mqttConnection::MQTTClient = PubSubClient(wifiConnection::wifiClient);

void mqttConnection::configureMQTTClient()
{
    mqttConnection::MQTTClient.setServer(BROKER.c_str(), PORT_CONNECTION);
    mqttConnection::MQTTClient.setCallback(mqttConnection::clientCallback);
    mqttConnection::reconnectMQTTClient();
}

void mqttConnection::reconnectMQTTClient()
{
    while (!MQTTClient.connected())
    {
        Serial.print("Attempting MQTT connection... ");
        if (MQTTClient.connect("Vault"))
        {
            Serial.println("Connected to broker.");
            MQTTClient.subscribe(TOPIC.c_str());
        }
        else
        {
            Serial.print("Retying in 5 seconds - failed, rc=");
            Serial.println(MQTTClient.state());

            delay(5000);
        }
    }
}

void mqttConnection::clientCallback(char *topic, uint8_t *payload, unsigned int length)
{
    char buff[length + 1];
    for (int i = 0; i < length; i++)
    {
        buff[i] = (char)payload[i];
    }
    buff[length] = '\0';

    Serial.print("Message received - ");
    Serial.println(buff);
    String message = buff;
}