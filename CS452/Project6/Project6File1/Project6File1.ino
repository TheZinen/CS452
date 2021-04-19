
//Sample Deep Sleep Code with Button Wake up
//Pullup works with this one

#include <WiFi.h>
#include <WiFiMulti.h>
//WiFi Connection
WiFiMulti wifiMulti;       // Create WiFiMulti object
#include <WiFiMulti.h>
#include "driver/rtc_io.h"  //need this for internal pull up

#define WAKE_PIN GPIO_NUM_32  //wake up pin
const int p_LED = 34;
const timer = 20;
int willGoAway = 1; //does not have RTC_DATA_ATTR - will be reinitialized after wake up
RTC_DATA_ATTR int willStay = 1; //will remain after wake up

RTC_DATA_ATTR int bootCount = 0;  //will remain after wake up

WiFiClient client;
unsigned long myChannelNumber = 974161;
const char * myWriteAPIKey = "XGA0ATODHKPNJIKN";
const char * myReadAPIKey = "U2TOTQ5GWPM8YEIR";
const char* server = "api.thingspeak.com";

void setup() {
  Serial.begin(115200); //use serial console for printing
  delay(100);

  if (bootCount == 0) { //call the following only at the first time
    oneTimeInit();
  }

  Serial.printf("Number of boots = %d\n", bootCount); //print number of times woken up

  bootCount++;  //increment

  initSystem(); //run this initialization every time - connect wifi
  runSystem();  //run this code - get data, puclush data, etc

  esp_sleep_enable_ext0_wakeup(WAKE_PIN, 0);

  Serial.println("Sleeping until button is pushed");

  rtc_gpio_pullup_en(WAKE_PIN);   //enable pull up
  esp_deep_sleep_start(); //go to sleep

}

//called only once
void oneTimeInit() {
  Serial.println("First time initialization");
}

//called each time waking up
void initSystem() {
  Serial.println("Initialization of system");
  wifiMulti.addAP("Jaylen","chan1318");
  wifiMulti.addAP("APUWIFI", "BrawlingFoxStyle64!");
  Serial.println("Connecting...");
  connectWiFi();

  pinMode(p_LED, OUTPUT);
  digitalWrite(p_LED, HIGH);
  pinMode(p_button, INPUT_PULLUP);
}

//called each time wake up
void runSystem() {
  willGoAway++;
  willStay++;
  Serial.printf("willGoAway = %d, willStay = %d\n", willGoAway, willStay);

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

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  // Prints the incoming message
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
 /* for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();  */
  //check for each one  
  for (int i = 0; i < sizeof(modules) / sizeof(Module); i++) {    
    if (!strcmp(topic, modules[i].topic)) {      
      modules[i].handler(message, length);      
      break;    
      }  
   }
}

//leav this empty
void loop() {
}

void connectWiFi() {
  Serial.print("MAC: ");
  Serial.println(WiFi.macAddress());
  while (wifiMulti.run() != WL_CONNECTED) { // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above
    delay(250);
    Serial.print('.');
    digitalWrite(p_LED, !digitalRead(p_LED));
  }
  digitalWrite(p_LED, LOW);

  Serial.println('\n');
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());               // Tell us what network we're connected to
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());            // Send the IP address of the ESP8266 to the computer
}
