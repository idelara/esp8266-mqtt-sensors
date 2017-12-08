// Blank ESP8266 - MQTT Connection

/*
This sketch does the following:
1) Sets up WiFi & the connection to the MQTT Server and registers a callback
2) Subscribes to a `testTopic` topic
3) Upon receiving a message from a server, executes the function callback()
*/

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>


// WiFi SSID. Basically, the name of your network
const char* ssid = "wifissd"; // Change this to match your network's SSID
// WiFi Password
const char* password = "1234567890"; // Change this to match your network's password
// Connection retry delay in millis
const int retryDelay = 5000;
// Address of your MQTT Server
const char* mqttServer = "192.168.31.13";
// Port of the MQTT Server. Usually it is port 1883
const int mqttServerPort = 1883;
// Baud rate
const int baudRate = 9600;

// WiFiClient instance
WiFiClient espClient;
// PubSubClient setting WiFi client
PubSubClient client(espClient);

// Callback that is executed upon receiving a message from our MQTT server
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.println("Callback!");
  // Print message topic
  Serial.print("Topic: ");
  Serial.println(topic);
  // Print length of message
  Serial.print("Length: ");
  Serial.println(length);
  // Print payload
  Serial.print("Payload: ");
  // Append NULL terminator at the end of the byte array to delimit our payload
  payload[length] = '\0';
  Serial.println((char*) payload);
}

// Attempts to connect to our MQTT server upon a failed connection
// NOTE: It executes at least once
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266 Client")) {
      Serial.println("connected");
      // Subscribe to a topic in our MQTT Server
      client.subscribe("testTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(retryDelay);
    }
  }
}

void setup()
{
  // Start Serial monitoring
  Serial.begin(baudRate);
  // Setup client
  client.setServer(mqttServer, mqttServerPort);
  client.setCallback(callback);
}

void loop()
{
  // If the client is not connected, retry until connection succeeds
  if (!client.connected()) {
    reconnect();
  }
  // Enter client loop
  client.loop();
}
