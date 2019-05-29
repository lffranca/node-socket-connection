#include "EspMQTTClient.h"

// Change the credentials below, so your ESP8266 connects to your router
const char* ssid = "VIVO-E040";
const char* password = "C66256E040";

// Change the variable to your Raspberry Pi IP address, so it connects to your MQTT broker
const char* mqtt_server = "192.168.15.6";
const uint8_t mqtt_port = 1883;
const char* mqtt_client_user = "test";
const char* mqtt_client_pass = "test";
const char* mqtt_client_id = "esp32_client";
const char* mqtt_topic = "test";

void onConnectionEstablished();

EspMQTTClient client(
  ssid,             // Wifi ssid
  password,             // Wifi password
  mqtt_server,    // MQTT broker ip
  mqtt_port,               // MQTT broker port
  mqtt_client_user,              // MQTT username
  mqtt_client_pass,         // MQTT password
  mqtt_client_id,            // Client name
  onConnectionEstablished, // Connection established callback
  true,               // Enable web updater
  true                // Enable debug messages
);

void setup()
{
  Serial.begin(115200);
}

void onConnectionEstablished()
{
  // Subscribe to mqtt_topic and display received message to Serial
  client.subscribe(mqtt_topic, [](const String &payload) {
    Serial.println(payload);
  });

  // Publish a message to mqtt_topic
  client.publish(mqtt_topic, "This is a message");

  // Execute delayed instructions
  client.executeDelayed(5 * 1000, []() {
    client.publish(mqtt_topic, "This is a message sent 5 seconds later");
  });
}

void loop()
{
  client.loop();
}
