#include <SimpleDHT.h>
#define Timer_read (3*1000)   // Defines our Timer Read 
long timer_last_read = 0;     // Defines the last time our timer read
long timer_now;               // Defines our current time.
int pinDHT11 = 4;             // Defines the pin we will use for DHT11
SimpleDHT11 dht11(pinDHT11);  // Connects the pin to the library we are calling from
byte temp = 0;                // Defines our temperature
byte humidity = 0;            // Defines our humidity

void setup() {
  Serial.begin(115200);       // Starts up the Arduino console
}

void loop() 
{
  timer_now = millis();       // Gets current time
  if (timer_now - timer_last_read >= Timer_read) {  // Loop function that changes time
    timer_last_read = timer_now;  // Sets the last time read as current time
    read();                   // Calls our read function
  }
}

//read function
void read() {
  // Starts our read function
  Serial.println("=================================");
  Serial.println("Starting Samples...");

  int err = SimpleDHTErrSuccess;// Defines our potential error
  if ((err = dht11.read(&temp, &humidity, NULL)) != SimpleDHTErrSuccess) // Function that tell us if our values are available
  {
    Serial.print("Read DHT11 failed, err=");      // Calls our error if the DHT11 fails
    Serial.println(err);                          // System prints out that error for us
    delay(1000);                                  // Delay so console doesn't get flooded
    return;                                       // Returns our values
  }
  
  Serial.print("Sample OK: ");                    // Otherwise, samples output
  Serial.print((int)temp); Serial.print(" *C, "); // Prints out our current temperature
  Serial.print((int)humidity); Serial.println(" H");  // Prints out our current humidity
  delay(1500);                                    // Delays so console doesn't get flooded
}
