//ESP32

const int T_wait = 1000;
const int p_LED = 2;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(10);

  Serial.println("Start...");
}

boolean f_got_resp = true;

void loop() {
  // put your main code here, to run repeatedly:

  if (f_got_resp == true) {
  Serial.println("what is you name?");
  }
  // Message buffer
  char message[256];

  int i_msg = Serial.readBytesUntil('\n', message, 255);
  message[i_msg] = '\0';
  if (i_msg > 0) {
  Serial.print("Hello ");
  Serial.println(message);
  Serial.println();
  f_got_resp = true;
  }
}
