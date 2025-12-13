//Define all Arduino Pins

//Digital Outputs
#define USER_LED1 9
#define USER_LED2 10
#define USER_LED3 11
#define USER_LED4 12
#define BUCK_PIN 6

//Digital Inputs
#define USER_BTN1 3
#define USER_BTN2 4

//Analog Inputs
#define INDUCTOR_CURR A1
#define USER_POTI A2
#define VIN_MEAS A3
#define VOUT_MEAS A4

//Constants
const double pwm_freq = 150000; //PWM Frequency in Hz
const double adc_max = 4095; //Max Value of ADC -->12bit
const double v_ref = 5.0;   //Analog Reference Voltage

//Functions and global Variables
float getVoltage(unsigned int pin)
{
  return analogRead(pin)*(v_ref/adc_max);
}

double dutycycle = 0.3*getAnalogWriteMaximum(); //Initial Duty Cycle of Arduino


void setup()
{
  //Pin Setups
  pinMode(BUCK_PIN, OUTPUT);
  pinMode(USER_LED1, OUTPUT);
  pinMode(USER_LED2, OUTPUT);
  pinMode(USER_LED3, OUTPUT);
  pinMode(USER_LED4, OUTPUT);
  pinMode(USER_BTN1, INPUT);
  pinMode(USER_BTN2, INPUT);

  //Start Serial Conversation
  Serial.begin(9600);
  Serial.println("Buck-converter program is starting");

  //Initialize All Pins
  // Initial Condition of Power Stage: Output is connected to GND
  digitalWrite(BUCK_PIN, LOW);
  //digitalWrite(BOOST_PIN, LOW);

  //Turn off all LEDs
  digitalWrite(USER_LED1, LOW);
  digitalWrite(USER_LED2, LOW);
  digitalWrite(USER_LED3, LOW);
  digitalWrite(USER_LED4, LOW);

  //Set PWM-Frequency
  setAnalogWriteFrequency(BUCK_PIN, pwm_freq);
  analogReadResolution(12);

  delay(100);
  analogWrite(BUCK_PIN, 255 - dutycycle); //write dutycycle in Arduino --> Real one will be inverted

}

void loop()
{
  //USER_BTN1 increases the duty-cycle of Arduino / decreases real duty-cycle after new Patch
  if(digitalRead(USER_BTN1))
  {
    dutycycle = dutycycle - 0.1*getAnalogWriteMaximum();
    Serial.print("Dutycycle: ");
    Serial.println(dutycycle);
    analogWrite(BUCK_PIN, 255 - dutycycle);
    //Blink LED 3
    digitalWrite(USER_LED3, HIGH);
    delay(100);
    digitalWrite(USER_LED3, LOW);
    //Delay for debouncing
    delay(500);
  }

  //USER_BTN2 decreases the duty-cycle of Arduino / increases real duty-cycle after new Patch
  if(digitalRead(USER_BTN2))
  {
    dutycycle = dutycycle + 0.1*getAnalogWriteMaximum();
    Serial.print("Dutycycle: ");
    Serial.println(dutycycle);
    analogWrite(BUCK_PIN, 255 - dutycycle);
    //Blink LED 4
    digitalWrite(USER_LED4, HIGH);
    delay(100);
    digitalWrite(USER_LED4, LOW);
    //Delay for debouncing
    delay(500);
  }

  //Print the output voltage
  Serial.print("Vout: ");
  Serial.print(getVoltage(VOUT_MEAS)*(4.7+22)/4.7);
  Serial.println(" V");
  Serial.print("Duty-Cycle: ");
  Serial.print(dutycycle);
  Serial.print(" (entspricht ");
  Serial.print(((dutycycle)/255.0)*100);
  Serial.println("%)");
  //Short Delay such that serial monitor is not completely flooded
  delay(100);

}
