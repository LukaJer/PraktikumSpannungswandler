#define USER_LED1 9
void setup() {
  // put your setup code here, to run once:
pinMode(USER_LED1, OUTPUT);
digitalWrite(USER_LED1, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
digitalWrite(USER_LED1, HIGH);
delay(250);
digitalWrite(USER_LED1, LOW);
delay(250);
}
