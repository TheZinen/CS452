//ESP32
//Jaylen Quizon / CS452

#define LED_BUILTIN 2;

const int LED_PIN = LED_BUILTIN;
const int TIME_UNIT = 150;

char message[] = "Hello World 1 2 3";
char dotsLines[] = "Goodbye World 1 2 3";

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  Serial.println("<Arduino is ready>");     // Displays a console message that lets the user know that our Arduino is ready to output

  pinMode(LED_PIN, OUTPUT);                 // Sets out output onto our LED light
  digitalWrite(LED_PIN, HIGH);              // Allows our LED to change between HIGH/LOW "ON/OFF"
}

void loop() {
  while(Serial.available() == 0) {}
  Serial.readString().toCharArray(message, 256);

  Serial.print(message);                    // Displays the message that we are translating in console
  Serial.print(" to morse code\n");
  startMorse();                             // Runs our function to change our text into morse code
  digitalWrite(LED_PIN, HIGH);              // Turns the LED on
}

void startMorse() {

  char* character_pointer = message;

  while (*character_pointer) {
    switch (*character_pointer) {
      case 'a':
      case 'A':
        Serial.print("A | .-\n");
        dot();
        dash();
        //Serial.println();
        //pulseChar(".-");
        break;
      case 'b':
      case 'B':
        Serial.print("B | -...\n");
        dash();
        dot();
        dot();
        dot();
        //Serial.println();
        //pulseChar("-...");
        break;
      case 'c':
      case 'C':
        Serial.print("C | -.-.\n");
        dash();
        dot();
        dash();
        dot();
        //Serial.println();
        //pulseChar("-.-.");
        break;
      case 'd':
      case 'D':
        Serial.print("D | -..\n");
        dash();
        dot();
        dot();
        //Serial.println();
        //pulseChar("-..");
        break;
      case 'e':
      case 'E':
        Serial.print("E | .\n");
        dot();
        //Serial.println();
        //pulseChar(".");
        break;
      case 'f':
      case 'F':
        Serial.print("F | ..-.\n");
        dot();
        dot();
        dash();
        dot();
        //Serial.println();
        //pulseChar("..-.");
        break;
      case 'g':
      case 'G':
        Serial.print("G | --.\n");
        dash();
        dash();
        dot();
        //Serial.println();
        //pulseChar("--.");
        break;
      case 'h':
      case 'H':
        Serial.print("H | ....\n");
        dot();
        dot();
        dot();
        dot();
        //Serial.println();
        //pulseChar("....");
        break;
      case 'i':
      case 'I':
        Serial.print("I | ..\n");
        dot();
        dot();
        //Serial.println();
        //pulseChar("..");
        break;
      case 'j':
      case 'J':
        Serial.print("J | .---\n");
        dot();
        dash();
        dash();
        dash();
        //Serial.println();
        //pulseChar(".---");
        break;
      case 'k':
      case 'K':
        Serial.print("K | -.-\n");
        dash();
        dot();
        dash();
        //Serial.println();
        //pulseChar("-.-");
        break;
      case 'l':
      case 'L':
        Serial.print("L | .-..\n");
        dot();
        dash();
        dot();
        dot();
        //Serial.println();
        //pulseChar(".-..");
        break;
      case 'm':
      case 'M':
        Serial.print("M | --\n");
        dash();
        dash();
        //Serial.println();
        //pulseChar("--");
        break;
      case 'n':
      case 'N':
        Serial.print("N | -.\n");
        dash();
        dot();
        //Serial.println();
        //pulseChar("-.");
        break;
      case 'o':
      case 'O':
        Serial.print("O | ---\n");
        dash();
        dash();
        dash();
        //Serial.println();
        //pulseChar("---");
        break;
      case 'p':
      case 'P':
        Serial.print("P | .--.\n");
        dot();
        dash();
        dash();
        dot();
        //Serial.println();
        //pulseChar(".--.");
        break;
      case 'q':
      case 'Q':
        Serial.print("Q | --.-\n");
        dash();
        dash();
        dot();
        dash();
        //Serial.println();
        //pulseChar("--.-");
        break;
      case 'r':
      case 'R':
        Serial.print("R | .-.\n");
        dot();
        dash();
        dot();
        //Serial.println();
        //pulseChar(".-.");
        break;
      case 's':
      case 'S':
        Serial.print("S | ...\n");
        dot();
        dot();
        dot();
        //Serial.println();
        //pulseChar("...");
        break;
      case 't':
      case 'T':
        Serial.print("T | -\n");
        dash();
        //Serial.println();
        //pulseChar("-");
        break;
      case 'u':
      case 'U':
        Serial.print("U | ..-\n");
        dot();
        dot();
        dash();
        //Serial.println();
        //pulseChar("..-");
        break;
      case 'v':
      case 'V':
        Serial.print("V | ...-\n");
        dot();
        dot();
        dot();
        dash();
        //Serial.println();
        //pulseChar("...-");
        break;
      case 'w':
      case 'W':
        Serial.print("W | .--\n");
        dot();
        dash();
        dash();
        //Serial.println();
        //pulseChar(".--");
        break;
      case 'x':
      case 'X':
        Serial.print("X | -..-\n");
        dash();
        dot();
        dot();
        dash();
        //Serial.println();
        //pulseChar("-..-");
        break;
      case 'y':
      case 'Y':
        Serial.print("Y | -.--\n");
        dash();
        dot();
        dash();
        dash();
        //Serial.println();
        //pulseChar("-.--");
        break;
      case 'z':
      case 'Z':
        Serial.print("Z | --..\n");
        dash();
        dash();
        dot();
        dot();
        //Serial.println();
        //pulseChar("--..");
        break;

      case '0':
        Serial.print("0 | -----\n");
        dash();
        dash();
        dash();
        dash();
        //Serial.println();
        //pulseChar("-----");
        break;
      case '1':
        Serial.print("1 | .----\n");
        dot();
        dash();
        dash();
        dash();
        dash();
        //Serial.println();
        //pulseChar(".----");
        break;
      case '2':
        Serial.print("2 | ..---\n");
        dot();
        dot();
        dash();
        dash();
        dash();
        //Serial.println();
        //pulseChar("..---");
        break;
      case '3':
        Serial.print("3 | ...--\n");
        dot();
        dot();
        dot();
        dash();
        dash();
        //Serial.println();
        //pulseChar("...--");
        break;
      case '4':
        Serial.print("4 | ....-\n");
        dot();
        dot();
        dot();
        dot();
        dash();
        //Serial.println();
        //pulseChar("....-");
        break;
      case '5':
        Serial.print("5 | .....\n");
        dot();
        dot();
        dot();
        dot();
        dot();
        //Serial.println();
        //pulseChar(".....");
        break;
      case '6':
        Serial.print("6 | -....\n");
        dash();
        dot();
        dot();
        dot();
        dot();
        //Serial.println();
        //pulseChar("-....");
        break;
      case '7':
        Serial.print("7 | --...\n");
        dash();
        dash();
        dot();
        dot();
        dot();
        //Serial.println();
        //pulseChar("--...");
        break;
      case '8':
        Serial.print("8 | ---..\n");
        dash();
        dash();
        dash();
        dot();
        dot();
        //Serial.println();
        //pulseChar("---..");
        break;
      case '9':
        Serial.print("9 | ----.\n");
        dash();
        dash();
        dash();
        dash();
        dot();
        //Serial.println();
        //pulseChar("----.");
        break;
      default:
        Serial.print("\n");
        space();
        //Serial.println();
        //pulseChar(" ");
        break;
    }
    ++character_pointer;
  }


}

void dot() {                      // Serial.print(".");
  digitalWrite(LED_PIN, LOW);     // Sets the output light to off
  delay(150);                     // Delay helps showcase the morse code
  digitalWrite(LED_PIN, HIGH);    // Sets the output light to high
  delay(450);                     // Delay helps showcase the morse code
}

void dash() {                     // Serial.print("-");
  digitalWrite(LED_PIN, LOW);     // Sets the output light to off
  delay(750);                     // Delay helps showcase the morse code
  digitalWrite(LED_PIN, HIGH);    // Sets the output light to high
  delay(450);                     // Delay helps showcase the morse code
}

void space() {
  digitalWrite(LED_PIN, HIGH);    // Sets the output light to high
  delay(1000);                    // Delay helps showcase the morse code
}
