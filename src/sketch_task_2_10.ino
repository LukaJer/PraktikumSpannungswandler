#define USER_LED1 9
#define BUCK_CNTRL 6

const int PWM_frequency = 50000; //define the PWM frequency to 50 kHz

void setup() {

pinMode(USER_LED1, OUTPUT);
digitalWrite(USER_LED1, LOW);
setAnalogWriteFrequency(USER_LED1, PWM_frequency);  //set the frequency to the value above
}

void loop() {

  analogWrite(USER_LED1, 0.1*getAnalogWriteMaximum());  //write a PWM-signal with dutycycle of 10%
  delay(1000);

}
