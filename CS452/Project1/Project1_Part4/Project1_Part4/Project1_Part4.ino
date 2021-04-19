//ESP32
//Jaylen Quizon / CS452

const char* characters = Serial.readBytesUntil('\n', message, 255);
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
}

const int T_UNIT = 250;
 
const char* message = "Jaylen\0";
 
 void setup()
{
  // Set up our output onto our LED
  pinMode(2, OUTPUT);
}

// Constants that we can call from
const int T_UNIT = 250;
const int DOT = T_UNIT;
const int DASH = 3 * T_UNIT;
const int SYMBOL_SPACE = T_UNIT;
const int LETTER_SPACE = 3 * T_UNIT - SYMBOL_SPACE;
const int WORD_SPACE = 7 * T_UNIT - LETTER_SPACE;

  void loop()
{
  // Gets the size of the message
  int size = strlen(message);

  //loop through the message
  for (int i = 0; i < size; i++)
  {
    // If statement that changes all values into lowercase letters than throws them into an array
    const char* ch = strchr(characters, tolower(message[i]));    
    // Assuming the user write something, if statement will automatically run
    if (ch != NULL)
    {
      int index = (int)(ch - characters);    
      const char* morseSymbols = mappings[index];
      int count = strlen(morseSymbols);

      // For loop that pushes the values of the letters
      for (int i = 0; i < count; i++)
      {
        digitalWrite(13, HIGH);

        // If/Else that outputs either a dot or dash
        int symbolTime;
        char symbol = morseSymbols[i];
        if (symbol == '.')
          symbolTime = DOT;
        else
          symbolTime = DASH; 

        // Outputs the symbols onto our wifi card
        delay(symbolTime);
        digitalWrite(13, LOW);
        delay(SYMBOL_SPACE);
      }
      delay(LETTER_SPACE);        
    }
  }
  delay(WORD_SPACE);
}
