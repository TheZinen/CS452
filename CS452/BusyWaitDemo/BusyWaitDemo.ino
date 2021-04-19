#include <Ticker.h>

long T_update = 17*1000;
long T_read = 2*1000;
long T_blink = 250;

//long t_last_update = 0;
//long t_last_read = 0;
//long t_last_blink = 0;

Ticker updateTicker;
Ticker readTicker;
Ticker blinkTicker;

const int p_LED = 2;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(10);
  Serial.print("Start");

  pinMode(p_LED, OUTPUT);

  updateTicker.attach_ms(T_update, update);
  readTicker.attach_ms(T_read, read);
  blinkTicker.attach_ms(T_blink, blink);
}

// long t_now;

void loop() {
  // put your main code here, to run repeatedly:
//  t_now = millis();
//  if (t_now - t_last_update >= T_update) {
//    t_last_update = t_now;
//    update();
//  }
//
//  if (t_now - t_last_read >= T_read) {
//    t_last_read = t_now;
//    read();
//  }
//
//  if (t_now - t_last_blink >= T_blink){
//    t_last_blink = t_now;
//    blink();
//  }
}

void update() {
  Serial.println("Update");
}

void read() {
  Serial.println("Read");
}

void blink() {
  digitalWrite(p_LED, !digitalRead(p_LED));
}
