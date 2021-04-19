//Sample Deep Sleep Code with Timer Wake up

const int p_LED = 34;
const timer = 20;
int willGoAway = 1; //does not have RTC_DATA_ATTR - will be reinitialized after wake up
RTC_DATA_ATTR int willStay = 1; //will remain after wake up

RTC_DATA_ATTR int bootCount = 0;  //will remain after wake up

#define SEC_TO_SLEEP 5  //seconds to sleep

void setup() {
  Serial.begin(115200); //use serial console for printing
  delay(100);

  if (bootCount == 0) { //call the following only at the first time
    oneTimeInit();
  }

  Serial.printf("Number of boots = %d\n", bootCount); //print number of times woken up

  bootCount++;  //increment

  initSystem(); //run this initialization every time - connect wifi
  runSystem();  //run this code - get data, puclush data, etc

  esp_sleep_enable_timer_wakeup(SEC_TO_SLEEP * 1000000);  //sleep for number of seconds

  Serial.printf("Sleeping for %d seconds\n", SEC_TO_SLEEP);
  esp_deep_sleep_start(); //go to sleep
}

//called only once
void oneTimeInit() {
  Serial.println("First time initialization");
}

//called each time waking up
void initSystem() {
  pinMode(p_LED, OUTPUT);
  digitalWrite(p_LED, HIGH);
  pinMode(p_button, INPUT_PULLUP);
}

//called each time wake up
void runSystem() {
  willGoAway++;
  willStay++;
  Serial.printf("willGoAway = %d, willStay = %d\n", willGoAway, willStay);
}

void esp_sleep_get_wakeup_cause() {
  esp_sleep_wakeup_cause_t wakeup_reason;
  wakeup_reason = esp_sleep_get_wakeup_cause();  
  if (wakeup_reason == ESP_SLEEP_WAKEUP_EXT0) 
    {    //code for key press  
      int stateButton = digitalRead(pinButton); //read the state of the button
        if(stateButton == 1) { //if is pressed
        digitalWrite(p_LED, HIGH); //write 1 or HIGH to led pin
    } 
  else if (wakeup_reason == ESP_SLEEP_WAKEUP_TIMER) 
    {    //code for timer  
      timer_now = millis();       // Gets current time
        if (timer_now - timer_last_read >= Timer_read) {  // Loop function that changes time
        timer_last_read = timer_now;  // Sets the last time read as current time
        esp_deep_sleep_start();
    }
  }
}

//leav this empty
void loop() {
}
