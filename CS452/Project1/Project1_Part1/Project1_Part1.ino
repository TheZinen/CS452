//ESP32
//Jaylen Quizon / CS452

int LED_BUILTIN = 2;                    // Define our LED

void setup() {
  pinMode (LED_BUILTIN, OUTPUT);        // Defines our LED as our output

}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);      // Set our output to turn on our LED
  delay(1000);                          // Delay helps show that LED is "blinking"
  digitalWrite(LED_BUILTIN, LOW);       // Set our output to turn of our LED
  delay(1000);                          // Delay helps show that LED is "blinking"

}
