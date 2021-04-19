#define Timer_read (30*1000)        // Defines our time read default 
 
long timer_last_read = 0;           // Defines our time last read
const int p_LED = 2;                // Defines our pin output

void setup() {
  Serial.begin(115200);             // Starts up the IDE
  delay(10);                        // Delays the messages
  Serial.print("Start");            // Outputs the message
  pinMode(p_LED, OUTPUT);           // Makes the pin active for usage
}

long timer_now;                     // Current Time

void loop() {
  timer_now = millis();             // Pulls the current time
  if (timer_now - timer_last_read >= Timer_read) {  // Loop that allows for us to read the current time
    timer_last_read = timer_now;    // Sets the last time as current time
    read();                         // Calls our method function
  }
}

void read() {                       // Function that allows us to read the current message
  Serial.println("Hello World");
}
