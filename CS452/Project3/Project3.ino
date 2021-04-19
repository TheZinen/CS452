#include <ArduinoJson.h>
#include <SimpleDHT.h>
//Arduino JSON
#include <ArduinoJson.h>
//WiFi Connection
#include <WiFi.h>
#include <WiFiMulti.h>
//WiFi Connection
WiFiMulti wifiMulti;       // Create WiFiMulti object

WiFiClient client;

unsigned long myChannelNumber = 974161;
const char * myWriteAPIKey = "XGA0ATODHKPNJIKN";
const char * myReadAPIKey = "U2TOTQ5GWPM8YEIR";
const char* server = "api.thingspeak.com";


const int pinLed = 2;

void setup() {
  Serial.begin(115200);         // Start the Serial communication to send messages to the computer
  delay(10);
  Serial.println("Start");
  pinMode(pinLed, OUTPUT);     // Initialize the pinLed pin as an output
  digitalWrite(pinLed, LOW);  // turn off LED

  //WiFi
  WiFi.mode(WIFI_MODE_STA);
  wifiMulti.addAP("Amped_TAPR2_2.4", "6aeb76df");   // add Wi-Fi networks you want to connect to
  wifiMulti.addAP("FreedomPop_350", "freedompop");   // add Wi-Fi networks you want to connect to
  wifiMulti.addAP("APU-MYDEVICES");

  //print mac address
  Serial.println("Connecting ...");
  connectWiFi();

  digitalWrite(pinLed, LOW);

}

long T_read = 20000;  //period of reading

void loop() {
  // put your main code here, to run repeatedly:

  if (client.connect(server, 80)) {

    digitalWrite(pinLed, HIGH);

    String url = String("/channels/") +
                 myChannelNumber +
                 "/feeds.json?api_key=" +
                 myReadAPIKey +
                 "&results=2";

    String htmlStr = String("GET ") + url + " HTTP/1.1\n" +
                     "Host: " + server + "\n" +
                     "Connection: close\n\n";
    client.print(htmlStr);

    if (client.println() == 0) {
      Serial.println(F("Failed to send request"));
      return;
    }

    // Check HTTP status
    char status[32] = {0};
    client.readBytesUntil('\r', status, sizeof(status));
    Serial.println(status);

    if (strcmp(status, "HTTP/1.1 200 OK") != 0) {
      Serial.print(F("Unexpected response: "));
      Serial.println(status);
      return;
    }

    // Skip HTTP headers
    char endOfHeaders[] = "\r\n\r\n";
    if (!client.find(endOfHeaders)) {
      Serial.println(F("Invalid response"));
      return;
    }

    // Allocate the JSON document
    // CAN use arduinojson.org/v6/assistant to compute the capacity.
    const size_t capacity = JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(2) + 1000;
    char json[capacity + 1];
    //go until '{'
    client.find("{");
    int len = 0;
    //add back the '{'
    json[len++] = '{';
    while (client.available() > 0) {
      json[len++] = client.read();
    }
    //put in the '\0'
    json[len] = '\0';
    Serial.println(json);

    //create json document
    DynamicJsonDocument doc(capacity);

    // Parse JSON object
    DeserializationError error = deserializeJson(doc, json);
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      return;
    }

    // Extract values
    Serial.println(F("Response:"));
    Serial.println(doc["channel"]["id"].as<long>());
    Serial.println(doc["feeds"][0]["field1"].as<float>(), 6);
    Serial.println(doc["feeds"][0]["field2"].as<float>(), 6);
    Serial.println(doc["feeds"][0]["field3"].as<float>(), 6);
    Serial.println(doc["feeds"][1]["field1"].as<float>(), 6);
    Serial.println(doc["feeds"][1]["field2"].as<float>(), 6);
    Serial.println(doc["feeds"][1]["field3"].as<float>(), 6);

    // Disconnect
    client.stop();
    digitalWrite(pinLed, LOW);

  }

  delay(T_read);  //wait 20 sec

  String postStr = String(myWriteAPIKey) +
                   "&field1=" + temperature +
                   "&field2=" + humidity +
                   "&field3=" + lightLevel +
                   "\r\n\r\n";

  String htmlStr = String("POST /update HTTP/1.1\n") +
                   "Host: " + thingSpeakURL + "\n" +
                   "Connection: close\n" +
                   "X-THINGSPEAKAPIKEY: " +
                   myWriteAPIKey "\n" +
                   "Content-Type: application/x-www-form-urlencoded\n" +
                   "Content-Length: " +
                   postStr.length() +
                   "\n\n" + postStr;

  client.print(htmlStr);
}

//connect to the WiFi
void connectWiFi() {
  Serial.print("MAC: ");
  Serial.println(WiFi.macAddress());
  while (wifiMulti.run() != WL_CONNECTED) { // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above
    delay(250);
    Serial.print('.');
    digitalWrite(pinLed, !digitalRead(pinLed));
  }
  digitalWrite(pinLed, HIGH);

  Serial.println('\n');
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());               // Tell us what network we're connected to
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());            // Send the IP address of the ESP8266 to the computer
}
