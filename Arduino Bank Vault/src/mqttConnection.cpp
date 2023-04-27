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
            MQTTClient.subscribe(topic.c_str());
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

bool mqttConnection::jsonCheck(uint8_t *payload, string phoneID) {
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, payload);

    if (error) {                                            // Serialization error
        Serial.println("Error deserializing JSON");
        return false;
    }

    if (phoneID != "") {                                    // If a phone ID is provided...
        if (!doc.containsKey("phoneID")) {                      // No phone ID
            Serial.println("JSON did not contain a phone ID");
            return false;
        }

        if (doc["phoneID"] != phoneID) {                        // Wrong phone ID
            Serial.println("Incorrect phone ID");
            return false;
        }
    }

    if (!doc.containsKey("vault")) {                        // No vault object
        Serial.println("No vault provided");
        return false;
    }

    JsonObject vaultInfo = doc["vault"].as<JsonObject>();
    if (!vaultInfo.containsKey("id")) {                     // No vault ID
        Serial.println("No vault id provided");
        return false;
    }

    if (!vaultInfo["id"] == vaultID) {                      // Wrong vault ID
        Serial.println("Not for this vault");
        return false;
    }

    return true;
}