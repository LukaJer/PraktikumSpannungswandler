//Define all Arduino Pins

//Digital Outputs
#define USER_LED1 9
#define USER_LED2 10
#define USER_LED3 11
#define USER_LED4 12
//#define BOOST_PIN ?
#define BUCK_PIN 6

//Digital Inputs
#define USER_BTN1 3
#define USER_BTN2 4
#define SHUTDOWN 8
#define USER_SW 13

//Analog Inputs
#define IN_CURR A0
#define INDUCTOR_CURR A1
#define USER_POTI A2
//#define OUT_CURR ?
#define VIN_MEAS A3
#define VOUT_MEAS A4
#define NTC A5

//Constants
const double pwm_freq = 50000; //PWM Frequency in Hz
const double adc_max = 4095; //Max Value of ADC
const double v_ref = 5.0;   //Analog Reference Voltage
//const double vout_sensor = 14.7; -->unnötig wegen Spannungsteiler bei uns

//Functions and global Variables
float getVoltage(unsigned int pin)
{
  return analogRead(pin)*(v_ref/adc_max);
}

double dutycycle = 0.25*getAnalogWriteMaximum(); //Initial Duty Cycle


void setup()
{
  //Pin Setups

  pinMode(BUCK_PIN, OUTPUT);
  //pinMode(BOOST_PIN, OUTPUT);
  pinMode(USER_LED1, OUTPUT);
  pinMode(USER_LED2, OUTPUT);
  pinMode(USER_LED3, OUTPUT);
  pinMode(USER_LED4, OUTPUT);
  pinMode(USER_BTN1, INPUT);
  pinMode(USER_BTN2, INPUT);
  pinMode(USER_SW, INPUT);

  //Start Serial Conversation
  Serial.begin(9600);
  Serial.println("Buck-converter program is starting");

  //Initialize All Pins
  // Initial Condition of Power Stage: Output is connected to GND
  digitalWrite(BUCK_PIN, LOW);
  //digitalWrite(BOOST_PIN, LOW);

  //Turn on all LEDs
  digitalWrite(USER_LED1, HIGH);
  digitalWrite(USER_LED2, HIGH);
  digitalWrite(USER_LED3, HIGH);
  digitalWrite(USER_LED4, HIGH);

  //Set PWM-Frequency
  setAnalogWriteFrequency(BUCK_PIN, pwm_freq);
  analogReadResolution(12);

  delay(100);
  analogWrite(BUCK_PIN, dutycycle);

}

void loop()
{
  //USER_BTN2 increases the duty-cycle
  if(digitalRead(USER_BTN2))
  {
    dutycycle += (1/16)*getAnalogWriteMaximum();
    Serial.print("Dutycycle: ");
    Serial.println(dutycycle);
    analogWrite(BUCK_PIN, dutycycle);
    //Blink LED 4
    digitalWrite(USER_LED4, LOW);
    delay(100);
    digitalWrite(USER_LED4, HIGH);
    //Delay for debouncing
    delay(500);
  }

  //USER_BTN1 decreases the duty-cycle
  if(digitalRead(USER_BTN1))
  {
    dutycycle -= (1/16)*getAnalogWriteMaximum();
    Serial.print("Dutycycle: ");
    Serial.println(dutycycle);
    analogWrite(BUCK_PIN, dutycycle);
    //Blink LED 3
    digitalWrite(USER_LED3, LOW);
    delay(100);
    digitalWrite(USER_LED3, HIGH);
    //Delay for debouncing
    delay(500);
  }

  //Print the output voltage
  Serial.print("Vout: ");
  Serial.print(getVoltage(VOUT_MEAS)*(4.7+22)/4.7);
  Serial.println(" V");
  //Short Delay such that serial monitor is not completely flooded
  delay(50);



}
