//Digital Outputs
#define USER_LED1 9
#define USER_LED2 10
#define USER_LED3 11
#define USER_LED4 12
#define BUCK_CNTRL 6

//Digital Inputs
#define USER_BTN1 3
#define USER_BTN2 4

//Analog Inputs
#define VOUT_MEAS A4

//Defining some Variables
const double R2 = 4.7;  //Resistor where we measure
const double R1 = 22;
const double K = R2/(R1+R2); //Transferfunction of the sensor, Voltage Divider, we need this as a prefilter for our desired output voltage
const double PWM_frequency = 150000;  //PWM frequency
const double adc_max = 4095;  //our maximum adc value

//Input of our Control Loop / "Führungsgröße"
double V_ref = 2;   //desired output voltage
double V_ref_filtered = V_ref * K;  //prefiltered output voltage --> now we can calculate v_err

//Declaring
double Vout_meas, V_err, dutycycle, integrator_sum = 0.0, current_integrator_sum;
double kI = 142; //value from simulation in MatLab
double kP = 0;  // we only use a I-Controller her
unsigned long end, begin; //variables to store some time stamps
unsigned long next_loop_time; //important variable for the while loop

double tsample_seconds = 0.001; //our sample time in seconds
unsigned long tsample_microseconds = (unsigned long)(tsample_seconds*1000000+0.5); //our sample time in microseconds (+0.5 because unsigned long cuts the decimals)


//Function to get an analog Voltage out of an read in adc value
float getAnalogVoltage(unsigned int pin)
{
  return analogRead(pin)*(5.0/adc_max);
}

//Setup
void setup()
{
  //Declaring as IN or OUTPUTS
  pinMode(USER_LED1, OUTPUT);
  pinMode(USER_LED2, OUTPUT);
  pinMode(USER_LED3, OUTPUT);
  pinMode(USER_LED4, OUTPUT);
  pinMode(BUCK_CNTRL, OUTPUT);

  pinMode(USER_BTN1, INPUT);
  pinMode(USER_BTN2, INPUT);

  //Serial.begin(9600);

  //Initializing all with LOW
  digitalWrite(USER_LED1, LOW);
  digitalWrite(USER_LED2, LOW);
  digitalWrite(USER_LED3, LOW);
  digitalWrite(USER_LED4, LOW);
  digitalWrite(BUCK_CNTRL, LOW);

  //Setting the PWM frequency and the Read Resolution
  setAnalogWriteFrequency(BUCK_CNTRL, PWM_frequency);
  analogReadResolution(12);

  //short delay
  delay(100);

  next_loop_time = micros(); //--> initializing with current time stamp so first time the while loop will be false
}

void loop()
{
  begin = micros();   //store current time stamp

  while(begin < next_loop_time) //is the current time stamp lower than the next fixed time to start the calculation loop? if yes-->wait so long until it is slightly bigger
  {
    begin = micros();
  }

  next_loop_time = next_loop_time + tsample_microseconds; //set thze next fixed time so start the calculation loop (for the next iteration)

  Vout_meas = getAnalogVoltage(VOUT_MEAS);  //read in the measured voltage
  V_err = V_ref_filtered - Vout_meas;   //calculate the error

  current_integrator_sum = integrator_sum + V_err * tsample_seconds;  //add to our integrated sum until now the new iteration sum and store it in the current_integrator_sum

  dutycycle = (kP * V_err + kI * current_integrator_sum);   //calculate the duty cycle as a decimal, not as an ADC
  if(dutycycle < 0)   //check if it is lower than 0
  {
    dutycycle = 0.0;  //then it should be 0
  }
  else if (dutycycle > 1)   //otherwise if its greater than 1
  {
    dutycycle = 1.0;    //then it should be 1
  }
  else    //so if we dont exceed the boundaries, we can set our integrated sum as the current sum
  {
    integrator_sum = current_integrator_sum;  
  }

  analogWrite(BUCK_CNTRL, getAnalogWriteMaximum()*(1-dutycycle)); //write the dutycycle in arduino inverted, so the real PWM will be right

  //Serial.print("Vout_meas: ");
  //Serial.println(Vout_meas);

  end = micros(); //store current time stamp
  if(end > next_loop_time)  //if our current time is greater than the time we should start the next iteration, then light up the LED so we know our program is too slow
  {
    digitalWrite(USER_LED1, HIGH);
  }
  else  //otherwise let the LED be LOW
  {
    digitalWrite(USER_LED1, LOW);
  } //so now we now if the LED is blinking, that our Program is nearly as fast as our sample time
}
