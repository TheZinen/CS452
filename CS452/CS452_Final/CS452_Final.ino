// All libraries included in this project
#include <PubSubClient.h>
#include <TimeLib.h>
#include <Time.h>
#include <TimeAlarms.h>
#include <WiFiUdp.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <WiFiMulti.h>

//WiFi Connection
WiFiMulti wifiMulti;       // Create WiFiMulti object
#include <WiFiMulti.h>

//MQTT Client
const char* mqtt_server = "broker.hivemq.com";
char* sub1 = "apu/cs495-02/quizon/alarmHours";
char* sub2 = "apu/cs495-02/quizon/alarmMinutes";
char* clientID = "QuizonESP1";
WiFiClient espClient;
PubSubClient client(espClient);

LiquidCrystal_I2C lcd(0x27, 20, 4);
const int ledPin = BUILTIN_LED;
const int buzzer = D4;
WiFiUDP ntpUDP;

// Network Time Protocol Servers:
IPAddress timeServer(91, 206, 8, 36);                   // 0.at.pool.ntp.org
const int timeZone = -7;                                // Pacific Daylight Time (USA)
WiFiUDP Udp;
unsigned int localPort = 8888;                          // local port to listen for UDP packets

void setup()                                            // Function that sets up our esp 32 for the first time
{
  Serial.begin(115200);
  connectwifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(MQTT_handler);
  Serial.println("MQTT client started");
  digitalWrite(BUILTIN_LED, LOW);

  client.setServer(mqtt_server, 1883);

  lcd.begin(16, 2);
  lcd.init();                                           // Turns on our LED module
  lcd.backlight();                                      // Turn backlight ON
  lcd.setCursor(0, 0);
  pinMode(D4, OUTPUT);
}

time_t prevDisplay = 0;                                 // variables that holds the current time

int button = 0;
int alarmHours = 0;
int alarmMinutes = 0;
int count = 0;

void loop()                                                       
{
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();
  if (alarmHours == hour() && alarmMinutes == minute())                     // Function that checks if a time interval was hit
  {                                                                         // If so, the alarm will go off
    lcd.clear();
    Serial.println("Wake Up!");
    lcd.println("Wake Up!");
    for (int i = 0; i < 10; i++)
    {
      buzz();
    }
    delay(5000);
    lcd.clear();
  }
  if (timeStatus() != timeNotSet)
  {
    if (now() != prevDisplay)                                               // Updates the times if they do not equal
    { 
      prevDisplay = now();
      digitalClockDisplay();
    }
  }
}

void buzz()        // Function that creates our alarm buzz
{
  tone(buzzer, 261);
  delay(2000);
  noTone(buzzer);
  delay(4000);
}

void digitalClockDisplay()                                        // Function that would display the time onto our clock
{
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(".");
  Serial.print(month());
  Serial.print(".");
  Serial.print(year());
  Serial.println();

  lcd.setCursor(0, 0);
  lcd.print("Time: ");
  lcd.print(hour());
  lcd.print(":");
  lcd.print(minute());
  lcd.print(":");
  lcd.print(second());
  lcd.setCursor(0, 1);
  lcd.print("Date: ");
  lcd.print(day());
  lcd.print("/");
  lcd.print(month());
  lcd.print("/");
  lcd.print(year());
}

void printDigits(int digits)                                                    // Prints out the digits that are being held
{
  Serial.print(":");
  if (digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

void connectwifi()
{
  delay(100);
  //WiFi
  wifiMulti.addAP("DonChanJay", "Imagine1924"); 			// home wifi
  wifiMulti.addAP("Jaylen", "1234"); 						// Mobile Hotspot

  Serial.println("Connecting ...");
  Serial.print("MAC: ");
  Serial.println(WiFi.macAddress());
  while (wifiMulti.run() != WL_CONNECTED)               // Looks and connects to the strongest wifi signal
  {
    delay(250);
    Serial.print('.');
    digitalWrite(ledPin, !digitalRead(ledPin));
  }
  digitalWrite(ledPin, LOW);

  Serial.println('\n');
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());               // Tells us the current wifi that we are connected to
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());            // Tells us the current IP of the device

  Serial.println("Starting UDP");
  Udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(Udp.localPort());
  Serial.println("waiting for sync");
  setSyncProvider(getNtpTime);
}

void reconnect()                                        // Function that connects the ESP to the wifi
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str()))               // if successful, connects us to our MQTT server
    {
      Serial.println("connected");
      client.subscribe(sub1);
      client.subscribe(sub2);
    }
    else                                                // if failed, will output that it did
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(6000);
    }
  }
} 

void MQTT_handler(char* topic, byte* payload, unsigned int length)                // Function that pulls the info from the server and displays it back
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  if (!strcmp(topic, sub1))
  {
    getalarmHours(payload, length);
  }
  if (!strcmp(topic, sub2))
  {
    getalarmMinutes(payload, length);
  }
}

const int NTP_PACKET_SIZE = 48;                                       // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE];                                   // buffer to hold incoming & outgoing packets

time_t getNtpTime()
{
  while (Udp.parsePacket() > 0) ;                                     // Disregards the packets before it
  Serial.println("Transmit NTP Request");
  sendNTPpacket(timeServer);                                          // Sends the time over 
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500)
  {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE)
    {
      Serial.println("Receive NTP Response");
      Udp.read(packetBuffer, NTP_PACKET_SIZE);                        // Reads the packets received from the buffer
      unsigned long secsSince1900;
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;   // Converts packets with time zone to return our time
    }
  }
  Serial.println("No NTP Response :-(");
  return 0;                                                              // if nothing passes through, returns nothing
}

void sendNTPpacket(IPAddress &address)                                   // Sends a packet request at our given address
{
  memset(packetBuffer, 0, NTP_PACKET_SIZE);                              // Buffers are set to 0
  // All NTP fields have been given values according to its wiki
  packetBuffer[0] = 0b11100011;                                           // LI, Version, Mode
  packetBuffer[1] = 0;                                                    // Stratum, or type of clock
  packetBuffer[2] = 6;                                                    // Polling Interval
  packetBuffer[3] = 0xEC;                                                 // Peer Clock Precision
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;
  Udp.beginPacket(address, 123);                                         // NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}

void getalarmMinutes(byte* payload, unsigned int length)                        // Function that works with the minutes of the clock
{

  if (length > 0)                                                               // Runs if the length is bigger than 0
  {
    switch (payload[0])                                                         // Different cases for the increment of minutes by 10
    {
      case '0':
        break;
      case '1':
        alarmMinutes = 10;
        break;
      case '2':
        alarmMinutes = 20;
        break;
      case '3':
        alarmMinutes = 30;
        break;
      case '4':
        alarmMinutes = 40;
        break;
      case '5':
        alarmMinutes = 50;
        break;
    }
    
    switch (payload[1])                                                         // Different cases for the increment of minutes by 1
    {
      case '0':
        break;
      case '1':
        alarmMinutes = alarmMinutes + 1;
        break;
      case '2':
        alarmMinutes = alarmMinutes + 2;
        break;
      case '3':
        alarmMinutes = alarmMinutes + 3;
        break;
      case '4':
        alarmMinutes = alarmMinutes + 4;
        break;
      case '5':
        alarmMinutes = alarmMinutes + 5;
        break;
      case '6':
        alarmMinutes = alarmMinutes + 6;
        break;
      case '7':
        alarmMinutes = alarmMinutes + 7;
        break;
      case '8':
        alarmMinutes = alarmMinutes + 8;
        break;
      case '9':
        alarmMinutes = alarmMinutes + 9;
        break;
    }
  }

  Serial.println(alarmMinutes);                                               // Outputs the clock's minutes
}

void getalarmHours(byte* payload, unsigned int length)                                  // Function that pushes the hours of the clock
{

  if (length > 0)                                                                       // Runs if the length is bigger than 0 
  {
    switch (payload[0])                                                                 // Different cases depending on the current hour (military time)
    {
      case '0':
        break;
      case '1':
        alarmHours = 10;
        break;
      case '2':
        alarmHours = 20;
        break;
    }
    switch (payload[1])                                                                 // Different cases depending on the current hour 
    {
      case '0':
        break;
      case '1':
        alarmHours = alarmHours + 1;
        break;
      case '2':
        alarmHours = alarmHours + 2;
        break;
      case '3':
        alarmHours = alarmHours + 3;
        break;
      case '4':
        alarmHours = alarmHours + 4;
        break;
      case '5':
        alarmHours = alarmHours + 5;
        break;
      case '6':
        alarmHours = alarmHours + 6;
        break;
      case '7':
        alarmHours = alarmHours + 7;
        break;
      case '8':
        alarmHours = alarmHours + 8;
        break;
      case '9':
        alarmHours = alarmHours + 9;
        break;
    }
  }

  Serial.println(alarmHours);                                                   // Outputs the clock hours
}
