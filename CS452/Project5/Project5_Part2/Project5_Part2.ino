#include <SPI.h>
#include <WiFi101.h>
#include <PubSubClient.h>
#include "DHT.h"
#define DHTPIN 2                           // DHT Sensor connected to digital pin 2.
#define DHTTYPE DHT11                      // Type of DHT sensor.
#define LIGHTPIN A0                        // Analog light sensor connected to analog pin A0.

DHT dht(DHTPIN, DHTTYPE);                  // Initialize DHT sensor.

char ssid[] = "APUWIFI";                    //  Change this to your network SSID (name).
char pass[] = "BrawlingFoxStyle64!";           // Change this to your network password.
char mqttUserName[] = "Jaylen";             // Use any name.
char mqttPass[] = "DCIVN83T00OFMITT";      // Change to your MQTT API Key from Account > MyProfile.   
char writeAPIKey[] = "XGA0ATODHKPNJIKN";    // Change to your channel write API key.
long channelID = 974161;                    // Change to your channel ID.
int fieldNumber = 4;                        // Field to use is publishing to only one field.

static const char alphanum[] ="0123456789"
                              "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                              "abcdefghijklmnopqrstuvwxyz";  // For random generation of client ID.


WiFiClient client;                           // Initialize the Wi-Fi client library.

PubSubClient mqttClient(client); // Initialize the PubSubClient library.
const char* server = "mqtt.thingspeak.com"; 

unsigned long lastConnectionTime = 0; 
const unsigned long postingInterval = 20L * 1000L; // Post data every 20 seconds.

void setup() {
  
  Serial.begin(9600);
  int status = WL_IDLE_STATUS;  // Set temporary Wi-Fi status.

  // Attempt to connect to Wi-Fi network.
  while (status != WL_CONNECTED) 
  {
    status = WiFi.begin(ssid, pass); // Connect to WPA/WPA2 Wi-Fi network.
    delay(5000);
  }

  Serial.println("Connected to wifi");
  mqttClient.setServer(server, 1883);   // Set the MQTT broker details.
}

void loop() {

 // Reconnect if MQTT client is not connected.
  if (!mqttClient.connected()) 
  {
    reconnect();
  }

  mqttClient.loop();   // Call the loop continuously to establish connection to the server.

  // If interval time has passed since the last connection, publish data to ThingSpeak.
  if (millis() - lastConnectionTime > postingInterval) 
  {
    mqttPublishFeed(); // Publish three values simultaneously.
    // mqttPublishField(fieldNumber); // Use this function to publish to a single field
  }
}

void reconnect() 
{
  char clientID[9];

  // Loop until reconnected.
  while (!mqttClient.connected()) 
  {
    Serial.print("Attempting MQTT connection...");
    // Generate ClientID
    for (int i = 0; i < 8; i++) {
        clientID[i] = alphanum[random(51)];
    }
    clientID[8]='\0';

    // Connect to the MQTT broker.
    if (mqttClient.connect(clientID,mqttUserName,mqttPass)) 
    {
      Serial.println("connected");
    } 
    else 
    {
      Serial.print("failed, rc=");
      // Print reason the connection failed.
      // See https://pubsubclient.knolleary.net/api.html#state for the failure code explanation.
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void mqttPublishFeed() {
  
  float t = dht.readTemperature(true); // Read temperature from DHT sensor.
  float h = dht.readHumidity();  // Read humidity from DHT sensor.
  int lightLevel = analogRead(LIGHTPIN); // Read voltage from light sensor.
  
  // Create data string to send to ThingSpeak.
  String data = String("field1=") + String(t, DEC) + "&field2=" + String(h, DEC) + "&field3=" + String(lightLevel, DEC);
  int length = data.length();
  const char *msgBuffer;
  msgBuffer=data.c_str();
  Serial.println(msgBuffer);
  
  // Create a topic string and publish data to ThingSpeak channel feed. 
  String topicString = "channels/" + String( channelID ) + "/publish/"+String(writeAPIKey);
  length = topicString.length();
  const char *topicBuffer;
  topicBuffer = topicString.c_str();
  mqttClient.publish( topicBuffer, msgBuffer );
  lastConnectionTime = millis();
}

void mqttPublishField(int fieldNum) {
  
  float t = dht.readTemperature(true); // Read temperature from DHT sensor.
  
  // Create data string to send to ThingSpeak.
  String data = String(t, DEC);
  int length = data.length();
  const char *msgBuffer;
  msgBuffer = data.c_str();
  Serial.println(msgBuffer);
  
  // Create a topic string and publish data to ThingSpeak channel feed. 
  String topicString ="channels/" + String( channelID ) + "/publish/fields/field" + String(fieldNum) + "/" + String(writeAPIKey);
  length = topicString.length();
  const char *topicBuffer;
  topicBuffer = topicString.c_str();
  mqttClient.publish( topicBuffer, msgBuffer );

  lastConnectionTime = millis();
}
