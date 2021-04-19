//ESP32

#include <WiFi.h>;

//****WiFi Section
// Replace with credentials as needed
const char* ssid     = "QuizonESP1";
const char* password = "hello123";

// Set WiFi server port number to 1111
const int WiFiPort = 1111;
WiFiServer server(WiFiPort);
WiFiClient client;
//****

const int T_wait = 1000;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(10);

  Serial.println("Start...");
  //****WiFi Section
  // Create Wi-Fi network with SSID and password
  Serial.print("Setting AP (Access Point)...");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  server.begin();
  //****WiFi Section
}

boolean f_got_resp = true;

void loop() {
  // put your main code here, to run repeatedly:

  //message buffer
  char message[256];

  //****WiFi Section
  if (!client) {
    client = server.available();
    if (client) {
      Serial.println("New Client...");          // print a message out in the serial port
      Serial.println();
      f_got_resp = true;
    }
  }

  if (client) {                             // If a new client connects,
    if (f_got_resp == true) {
      client.println("What is your name?");
      f_got_resp = false;
    }

    int i_msg = client.readBytesUntil('\n', message, 255);

    if (i_msg > 0) {
      message[i_msg] = '\0';
      client.print("Hello ");
      client.println(message);
      client.println();
      f_got_resp = true;
    }
  }
}
