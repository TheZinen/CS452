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
///////////////////////////////////////
const unsigned int LED_PIN = 2;
const unsigned int T_UNIT = 5;
const unsigned int Dotts = 1200/T_UNIT;
const unsigned int Dashess = 3*Dotts;

const int pin_led = 2;
//array that stores the String to be morsed
char message[25]; // make into a user input gather

void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(115200);
  delay(10);
  //Setting the Status LED
  pinMode(LED_PIN, OUTPUT);

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
  //Main function to morse the whole char array one by one.
   wifi();
   int charsRead; 
   morseTranslate();
  while (Serial.available() > 0) {                                             // Anything in Serial buffer check please
      charsRead = Serial.readBytesUntil('\n', message, sizeof(message) - 1);  // need to add null
      message[charsRead] = '\0';                                               // make it a string
      Serial.println(message);
    }

}

void morseTranslate() {
  //Use a for loop to go through each character in the message 
  for (int i = 0; i < sizeof(message); i++) {

    /*For each letter (A-Z) there is a corresponding function.
      For SPACE (' ') the pause is longer 'halt()', as specified in Morse Code standards.
      If there is an undefined symbol in the array, the LED will hyperventilate().
      After each letter, the LED will pause() as specified in the Morse Code standards.
      It's important, that the letters are uppercase.
    */
    switch (message[i]) {
      case 'A':
        morseA();
        break;
      case 'B':
        morseB();
        break;
      case 'C':
        morseC();
        break;
      case 'D':
        morseD();
        break;
      case 'E':
        morseE();
        break;
      case 'F':
        morseF();
        break;
      case 'G':
        morseG();
        break;
      case 'H':
        morseH();
        break;
      case 'I':
        morseI();
        break;
      case 'J':
        morseJ();
        break;
      case 'K':
        morseK();
        break;
      case 'L':
        morseL();
        break;
      case 'M':
        morseM();
        break;
      case 'N':
        morseN();
        break;
      case 'O':
        morseO();
        break;
      case 'P':
        morseP();
        break;
      case 'Q':
        morseQ();
        break;
      case 'R':
        morseR();
        break;
      case 'S':
        morseS();
        break;
      case 'T':
        morseT();
        break;
      case 'U':
        morseU();
        break;
      case 'V':
        morseV();
        break;
      case 'W':
        morseW();
        break;
      case 'X':
        morseX();
        break;
      case 'Y':
        morseY();
        break;
      case 'Z':
        morseZ();
        break;
      
      case ' ': //'halt()' makes spaces pause longer
        intergap(); //intergap
        break;
      //If there is an undefined symbol in the array, the LED will hyperventilate().
      default:
        shortgap(); // shortgap()
    }
    //After each letter, the LED will pause() as specified in the Morse Code standards.
    mediumgap(); // medium pause
  }
}

//Morse functions for each letter (A-Z).

void morseA() {
  Dots();
  Dashes();
}

void morseB() {
  Dashes();
  Dots();
  Dots();
  Dots();
}

void morseC() {
  Dashes();
  Dots();
  Dashes();
  Dots();
}

void morseD() {
  Dashes();
  Dots();
  Dots();
}

void morseE() {
  Dots();
}

void morseF() {
  Dots();
  Dots();
  Dashes();
  Dots();
}

void morseG() {
  Dashes();
  Dashes();
  Dots();
}

void morseH() {
  Dots();
  Dots();
  Dots();
  Dots();
}

void morseI() {
  Dots();
  Dots();
}

void morseJ() {
  Dots();
  Dashes();
  Dashes();
  Dashes();
}

void morseK() {
  Dashes();
  Dots();
  Dashes();
}

void morseL() {
  Dots();
  Dashes();
  Dots();
  Dots();
}

void morseM() {
  Dashes();
  Dashes();
}

void morseN() {
  Dashes();
  Dots();
}

void morseO() {
  Dashes();
  Dashes();
  Dashes();
}

void morseP() {
  Dots();
  Dashes();
  Dashes();
  Dots();
}

void morseQ() {
  Dashes();
  Dashes();
  Dots();
  Dashes();
}

void morseR() {
  Dots();
  Dashes();
  Dots();
}

void morseS() {
  Dots();
  Dots();
  Dots();
}

void morseT() {
  Dashes();
}

void morseU() {
  Dots();
  Dots();
  Dashes();
}

void morseV() {
  Dots();
  Dots();
  Dots();
  Dashes();
}

void morseW() {
  Dots();
  Dashes();
  Dashes();
}

void morseX() {
  Dashes();
  Dots();
  Dots();
  Dashes();
}

void morseY() {
  Dashes();
  Dots();
  Dashes();
  Dashes();
}

void morseZ() {
  Dashes();
  Dashes();
  Dots();
  Dots();
}

//Morse functions for dits and dahs, as well as pause(), halt() and hyperventilate().

void Dots () {
  digitalWrite(LED_PIN, HIGH);
  delay(Dotts);
  digitalWrite(LED_PIN, LOW);
  delay(Dotts);
}

void Dashes () {
  digitalWrite(LED_PIN, HIGH);
  delay(Dashess);
  digitalWrite(LED_PIN, LOW);
  delay(Dashess);
}

void mediumgap() {
  delay(Dashess);
}

void intergap() {
  delay(7*Dotts);
}

void shortgap() {
  for (int i = 0; i < 5; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(80);
    digitalWrite(LED_PIN, LOW);
    delay(80);
  }
}
void wifi() {
 //message buffer
  char messagee[256];
  
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
      client.println("Enter the code");
      f_got_resp = false;
    }
    
    int i_msg = client.readBytesUntil('\n', messagee, 255);

    if (i_msg > 0) {
      message[i_msg] = '\0';
      client.print("The code is... ");
      client.println(messagee);
      client.println();
      f_got_resp = true;
    }
  }
}
