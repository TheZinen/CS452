//ESP32
//Jaylen Quizon / CS452

#define LED_BUILTIN 2;

const char* characters = "abcdefghijklmnopqrstuvwxyz0123456789";
const char* message = "codeproject\0";
const int LED_PIN = LED_BUILTIN;
const int TIME_UNIT = 250;
const int DOT = TIME_UNIT;
const int DASH = 3 * TIME_UNIT;
const int SYMBOL_SPACE = TIME_UNIT;
const int LETTER_SPACE = 3 * TIME_UNIT - SYMBOL_SPACE;
const int WORD_SPACE = 7 * TIME_UNIT - LETTER_SPACE;

const char* ATable[] = {
  ".-\0",     //a
  "-...\0",   //b
  "-.-.\0",   //c
  "-..\0",    //d
  ".\0",      //e
  "..-.\0",   //f
  "--.\0",    //g
  "....\0",   //h
  "..\0",     //i
  ".---\0",   //j
  "-.-\0",    //k
  ".-..\0",   //l
  "--\0",     //m
  "-.\0",     //n
  "---\0",    //o
  ".--.\0",   //p
  "--.-\0",   //q
  ".-.\0",    //r
  "...\0",    //s
  "-\0",      //t
  "..-\0",    //u
  "...-\0",   //v
  ".--\0",    //w
  "-..-\0",   //x
  "-.--\0",   //y
  "--..\0",   //z
};

const char* NTable[] = {
  "-----\0",  //0
  ".----\0",  //1
  "..---\0",  //2
  "...--\0",  //3
  "....-\0",  //4
  ".....\0",  //5
  "-....\0",  //6
  "--...\0",  //7
  "---..\0",  //8
  "----.\0",  //9
};

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_PIN, OUTPUT);
}
void loop()
{
  int size = strlen(message);
  //loop through the message
  for (int i = 0; i < size; i++)
  {
    const char* ch = strchr(characters, tolower(message[i]));
    if (ch != NULL)
    {
      int index = (int)(ch - characters);
      const char* morseSymbols = ATable[index];
      const char* morseNumbers = NTable[index];
      int count = strlen(morseSymbols);
      for (int i = 0; i < count; i++)
      {
        digitalWrite(13, HIGH);
        int symbolTime;
        char symbol = morseSymbols[i] + morseNumbers[i];
        if (symbol == '.')
          symbolTime = DOT;
        else
          symbolTime = DASH;
        delay(symbolTime);
        digitalWrite(13, LOW);
        delay(SYMBOL_SPACE);
      }
      delay(LETTER_SPACE);
    }
  }
  delay(WORD_SPACE);
}
