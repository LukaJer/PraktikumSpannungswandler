#define USER_LED1 9
#define BUCK_CNTRL 6

const int PWM_frequency = 50000; //Hz;

void setup() {

pinMode(USER_LED1, OUTPUT);
digitalWrite(USER_LED1, LOW);
setAnalogWriteFrequency(USER_LED1, PWM_frequency);
}

void loop() {

  analogWrite(USER_LED1, 0.1*getAnalogWriteMaximum());
  delay(1000);

}
