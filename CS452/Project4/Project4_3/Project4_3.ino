//CS452 demo code to avoid busy waits by hardware timer
//
//Asynchronously do the following
// 1) Update every 17 seconds
// 2) Read every 2 seconds
// 3) blink led every 250 msec

#include <Ticker.h> //Ticker library

//Periods - use T_ prefix for periods
long T_update = 17*1000;  //use long for anythign to do with time, time in msec
long t_now;
long t_last_read;
#define T_read (2*1000)   //can use #define, but need () and 
const long T_blink = 250; //use const if not going to change it
byte temp = 0;                // Defines our temperature
byte humidity = 0;            // Defines our humidity

//Ticker instances for each function
Ticker updateTicker;  //update ticker
Ticker readTicker;  //read ticker
Ticker blinkTicker; //blink ticker

const int p_LED = 2;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(10);
  Serial.print("Start");
  pinMode(p_LED, OUTPUT);

  //set the Tickers
  updateTicker.attach_ms(T_update, update); //set ticker to call update every T_update msec
  readTicker.attach_ms(T_read, read); //set ticker to call read every T_read msec
  blinkTicker.attach_ms(T_blink, blink); //set ticker to call blink every T_blink msec
}

void loop() {
  // put your main code here, to run repeatedly:
  t_now = millis();
  if (t_now - t_last_read >= T_read) {
    t_last_read = t_now;
    read();
  }
}

//update funciton
void update() {
  long T_last_update = T_update();
  Serial.println(T_update);
  Serial.println("UPDATE");
  blink();
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
  delay(1500);                                    // Delays so console doesn't get flood
  Serial.println("READ");
  update();
}

//blink function
void blink() {
  digitalWrite(p_LED, !digitalRead(p_LED));
}
