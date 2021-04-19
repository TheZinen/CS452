#define ESP32

#define BUILTIN_LED 2

//***SECTION***
//Headers
//WiFi
#include <WiFi.h>
//Web Server
#include <WebServer.h>
//following is required because we use WebServer object
extern WebServer server;
//WiFi Connection
#include <WiFiMulti.h>
//mDNS
#include <ESPmDNS.h>
//Servo
#include <ESP32Servo.h>


//LCD Web handler
void handleLCD() {
//**** CODE TO HANDLE LCD
}

//init LCD
void initLCD() {
//**** CODE TO INIT LCD
}


//***SECTION***
//WiFi Object
// Replace with credentials as needed
const char* ssid     = "QuizonESP1";
const char* password = "hello123";

//init WiFi
void initWiFi() {
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Setting AP (Access Point)...");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  //print IP address
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
//**** CODE TO WRITE TO LCD
}


//***SECTION***
const char* DNSname = "QuizonESP1";
//init mDNS
void initmDNS() {
    if (!MDNS.begin(DNSname)) {             // Start the mDNS responder for esp32.local
    Serial.println("Error setting up MDNS responder!");
  }
  Serial.println("mDNS responder started");
//**** CODE TO WRITE TO LCD
}


//***SECTION***
////Servo Object
Servo myservo;
//Servo variables
const int pin_servo = 13;
int servoPos = 90;

//init Servo
void initServo() {  
  myservo.write(servoPos);
  myservo.attach(pin_servo);
}

//Servo Web Handler
void handleServo() {
  char message[256];

  int degree = server.arg("angle").toInt();
//**** CODE TO WRITE TO LCD

  myservo.write(degree);
  servoPos = degree;

  server.sendHeader("Location", "/");       // Add a header to respond with a new location for the browser to go to the home htmlPage again
  server.send(303);                         // Send it back to the browser with an HTTP status 303 (See Other) to redirect
}

//Servo Page
String pageServo() {
  return String("<form action='/servo'>\n")
         + "  Servo Angle:<br>\n"
         + "  <input type='range' name='angle' min='0' max='180' value='" + servoPos + "'\n"
         + "    list='degrees' oninput='updateAngle(value)' id='angle'>\n"

         + "  <output for='angle' id='degree'>" + servoPos + "</output> deg\n"
         + "  <br>\n"

         + "  <input type='Submit'>\n"

         + "</form>\n"

         + "<datalist id='degrees'>\n"
         + "  <option>0</option>\n"
         + "  <option>45</option>\n"
         + "  <option>90</option>\n"
         + "  <option>135</option>\n"
         + "  <option>180</option>\n"
         + "</datalist>\n"

         + "<script>\n"
         + "function updateAngle(deg) {\n"
         + "  document.querySelector('#degree').value = deg;\n"
         + "}\n"
         + "</script>\n\n"
         ;
}


//***SECTION***
//Buzzer
const int p_buzzer = 12;


//***SECTION***
//Morse Code
//Length in msec of a t_unit
int t_unit = 120;  //10 wpm

//Morse Code Handler
void handleMorse() {
  char message[256];

  int msec = server.arg("speed").toInt();
  t_unit = msec;

  server.arg("message").toCharArray(message, 256);

  sendMorse(message);
  server.sendHeader("Location", "/");       // Add a header to respond with a new location for the browser to go to the home htmlPage again
  server.send(303);                         // Send it back to the browser with an HTTP status 303 (See Other) to redirect
}

//Morse Code Page
String pageMorse() {
  return String("<form action='/morse'>\n")
  //code for wpm here
         + "  Morse Code:<br>\n"
         + "  <input type='text' name='message'>\n"
         + "  <br>\n"
         + "  <input type='Submit'>\n"
         + "</form>\n"
         + "<br>\n\n"
         ;
}

String pageLCD() {
  return String("<form action='/lcd'>\n")
   
         + "  LCD:<br>\n"
         + "  <input type='text' name='message'>\n"
         + "  <br>\n"
         + "  <input type='Submit'>\n"
         + "</form>\n"
         + "<br>\n\n"
         ;
}

//Morse Code Alphabet Table
const char* ATable[] = {
  ".-",   //A
  "-...", //B
  "-.-.", //C
  "-..",  //D
  ".",    //E
  "..-.", //F
  "--.",  //G
  "....", //H
  "..",   //I
  ".---", //J
  "-.-",  //K
  ".-..", //L
  "--",   //M
  "-.",   //N
  "---",  //O
  ".--.", //P
  "--.-", //Q
  ".-.",  //R
  "...",  //S
  "-",    //T
  "..-",  //U
  "...-", //V
  ".--",  //W
  "-..-", //X
  "-.--", //Y
  "--.."  //Z
};

//Morse Code Numeric Table
const char* NTable[] = {
  "-----",  //0
  ".----",  //1
  "..---",  //2
  "...--",  //3
  "....-",  //4
  ".....",  //5
  "-....",  //6
  "--...",  //7
  "---..",  //8
  "----."   //9
};

//emit a short_mark
void dot() {
  digitalWrite(BUILTIN_LED, HIGH);
  digitalWrite(p_buzzer, HIGH);
  delay(t_unit);
}

//emit a dash
void dash() {
  digitalWrite(BUILTIN_LED, HIGH);
  digitalWrite(p_buzzer, HIGH);
  delay(3 * t_unit);

}

//emit inter-element gap
void inter_gap() {
  digitalWrite(BUILTIN_LED, LOW);
  digitalWrite(p_buzzer, LOW);
  delay(t_unit);
}

//emit short gap
void short_gap() {
  digitalWrite(BUILTIN_LED, LOW);
  digitalWrite(p_buzzer, LOW);
  delay(3 * t_unit);
}

//emit long gap
void medium_gap() {
  digitalWrite(BUILTIN_LED, LOW);
  digitalWrite(p_buzzer, LOW);
  delay(7 * t_unit);
}

//send Morse Code for a char* string
void sendMorse(char* message) {
//**** CODE TO WRITE TO LCD

  Serial.println(message);

  //loop through the message string
  for (int i = 0; message[i] != 0; i++) {
    char c = message[i];
    Serial.print(" ");
    Serial.print(c);

//**** CODE TO WRITE TO LCD
    //code for the character - set to null
    const char *code = 0;

    //check if alphanumeric and set code
    if (c >= 'A' && c <= 'Z') {
      //look in alphabet table
      code = ATable[c - 'A'];
    } else if (c >= 'a' && c <= 'z') {
      //look in alphabet table
      code = ATable[c - 'a'];
    } else if (c >= '0' && c <= '9') {
      //look in number table
      code = NTable[c - '0'];
    }

    //if not alphanumeric
    if (code == 0) {
      //check for space
      if (c == ' ') {
        //emit medium gap between words
        medium_gap();
        Serial.println();
      }

    } else {
      //short gap between if not first character
      if (i > 0) {
        short_gap();
      }

      //go through each of the code elements
      for (int j = 0; code[j] != 0; j++) {

        //put gap in if not first element
        if (j > 0)
          inter_gap();

        char d = code[j];
        Serial.print(d);


        if (d == '-' || d == '_') {
//**** CODE TO WRITE TO LCD
          dash();
        }
        else if (d == '.') {
//**** CODE TO WRITE TO LCD
          dot();
        }
      }
      Serial.println();
    }
  }

  //put a space after each message
  medium_gap();
  Serial.println();
}


//***SECTION***
//Web Server Section
//Web Server Object
const int serverPort = 80;      // Server port - can be specified by user
WebServer server(serverPort);    // Create WebServer object that listens for HTTP request

//init Web Server
void initWebServer() {
  addServers();
  server.begin();                           // Actually start the server
  Serial.println("HTTP server started");
//**** CODE TO WRITE TO LCD
}

//define Module structure
struct Module {
  char* name;
  String (&page)();
  void (&handler)();
};

//modules
Module modules[] = {
  {"/", pageRoot, handleRoot},
  {"/morse", pageMorse, handleMorse},
  {"/lcd",  pageLCD,  handleLCD},
  {"/servo", pageServo, handleServo}
};

//add servers
void addServers() {
  server.onNotFound(handleNotFound);        // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"
//  server.on("/", handleRoot);        // Call the 'handleRoot' function when a client requests URI "/"

  //start from zero
  for (int i = 0; i < sizeof(modules)/sizeof(Module); i++) {
    server.on(modules[i].name, modules[i].handler);
  }
}

//Not Found handler
void handleNotFound() {
  server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}

//Root page
String pageRoot() {
  String temp = "";
  
  //start from 1
  for (int i = 1; i < sizeof(modules)/sizeof(Module); i++) {
    temp += modules[i].page();
  }
  return temp;
}

//Root handler
void handleRoot() {                         // When URI / is requested, send a web htmlPage with a button to toggle the LED
  //serve page
  server.send(200, "text/html", pageRoot());
}


//***SECTION***
//Arduino
//Arduino initialization
void setup(void) {
  Serial.begin(115200);         // Start the Serial communication to send messages to the computer
  delay(10);
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  pinMode(p_buzzer, OUTPUT);          // Initialize buzzer

  initLCD();
  initServo();
  initWiFi();
  initmDNS();  
  initWebServer();
}

//Arduino loop
void loop(void) {
  server.handleClient();                    // Listen for HTTP requests from clients
}
