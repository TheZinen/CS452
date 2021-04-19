
#include <WiFiMulti.h>
#include <PubSubClient.h>
const int p_LED = 2;
WiFiMulti wifiMulti;
const char* mqtt_Server = "broker.hivemq.com";
const int mqtt_port = 1883;

WiFiClient wifi_client;
void mqtt_callback(char* topic, byte* payload, unsigned int length);
PubSubClient mqtt_client(mqtt_server, mqtt_port, mqtt_callback, wifi_client);

char* pub_topic = "apu/cs452/yeh/button";
char* sub_topic = "apu/cs495-02/Quizon/morse";
char* sub_topic2 = "apu/cs495-02/Quizon/speed";
char* sub_topic3 = "apu/cs495-02/Quizon/lcd";
char* clientID = "lkjhCLKHSSFUHr3y8w3r89pyu8";

//define Module structure
struct Module {  
  char* topic;  
  void (&handler)(byte*, unsigned int);
 };
  
  //modules
  Module modules[] = {  
    {"apu/cs495-02/yeh/morse", handleMorse},  
    {"apu/cs495-02/yeh/speed", handleSpeed},  
    {"apu/cs495-02/yeh/lcd", handleLCD},  
    {"apu/cs495-02/yeh/servo", handleServo}
   };

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(100);
  Serial.println("Project 5 Part 1");
  Serial.println("");
  Serial.println("(c) 2020 Jaylen Quizon");
  Serial.println();

  wifiMulti.addAP("Jaylen","chan1318");
  wifiMulti.addAP("APUWIFI", "BrawlingFoxStyle64!");
  Serial.println("Connecting...");
  connectWiFi();

  pinMode(p_LED, OUTPUT);
  digitalWrite(p_LED, HIGH);
  pinMode(p_button, INPUT_PULLUP);

  //connect to MQTT
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

 //if it is the correct one – use for loop to check each one  
 if (!strcmp(topic, subTopic)) 
 {    
  doSomthing(payload, length);  
 }

void loop() {
  // put your main code here, to run repeatedly:
  if(!mqtt_client.connected()) {
    mqtt_reconnect();
  }
  mqtt_client.loop();

  long t_now = millis();
  if (t_now - t_last_pub >= T_pub) {
    t_last_pub = t_now;
    
    S_button = digitalRead(p_button);

    Serial.print("Button: ");
    Serial.println(S_button);
    
    char message[256];
    snprintf(message, 255, "Button: %d", S_button);

    mqtt_client.publish(pub_topic, message);
  }
}

void mqtt_reconnect() {
  // Loop until we're reconnected
  while (!mqtt_client.connected()) {

    Serial.print("Attempting MQTT connection...");
    // Connect MQTT
    // if (client.connect(clientID, username, password)) {
    if (mqtt_client.connect(clientID)) {
      Serial.println("connected");
      //Subscribe
      mqtt_client.subscribe(sub_topic, 1);
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqtt_client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
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
