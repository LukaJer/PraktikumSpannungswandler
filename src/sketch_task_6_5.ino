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
#define USER_POTI A2
#define VOUT_MEAS A4

//Defining some Variables
const double R2 = 4.7;  //Resistor where we measure
const double R1 = 22;
const double K = R2/(R1+R2); //Transferfunction of the sensor, Voltage Divider, we need this as a prefilter for our desired output voltage
const double PWM_frequency = 150000;  //PWM frequency

//Initial Input of our Control Loop / "Führungsgröße"
double V_ref = 2.5;   //desired output voltage
double V_ref_filtered = V_ref * K;  //prefiltered output voltage --> now we can calculate v_err

//Declarations
double Vout_meas;
double V_err;
double potvolt;
double dutycycle;
double dutycycle_command;
double integrator_sum = 0.0;
double current_integrator_sum;
double kI = 641; //value from simulation in MatLab
double kP = 0;  // we only use a I-Controller here
unsigned long end; //variable to store a time stamp
unsigned long next_loop_time; //important variable for the while loop to make sure we only start at fixed sample time

double tsample_seconds = 0.00022; //our fixed sample time in seconds
unsigned long tsample_microseconds = (unsigned long)(tsample_seconds*1000000+0.5); //our fixed sample time in microseconds (+0.5 because unsigned long cuts the decimals)

//Set the Write and Reading Resolutions
const int adc_read_resolution = 10;
const int adc_write_resolution = 8;

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

  //Initializing all with LOW
  digitalWrite(USER_LED1, LOW);
  digitalWrite(USER_LED2, LOW);
  digitalWrite(USER_LED3, LOW);
  digitalWrite(USER_LED4, LOW);
  digitalWrite(BUCK_CNTRL, LOW);

  //Setting the PWM frequency and the reading resolution
  setAnalogWriteFrequency(BUCK_CNTRL, PWM_frequency);
  analogReadResolution(adc_read_resolution);
  analogWriteResolution(adc_write_resolution);

  //Serial Monitor
  Serial.begin(9600);
  Serial.setTimeout(5); //dont wait long for reading in*/

  //short delay
  delay(100);

  next_loop_time = micros(); //--> initializing with current time stamp so first time the while loop will be false
}

//Getting the Max Values for the ADC / DAC
const int adc_read_max = (1<< adc_read_resolution) - 1;
const int adc_write_max = (1<< adc_write_resolution) -1;
bool saturation = false;

//Function to get an analog voltage out of an read-in adc value
float getAnalogVoltage(unsigned int pin)
{
  return analogRead(pin)*(5.0/adc_read_max);
}

void loop()
{
  digitalWrite(USER_LED2, LOW); //to end measuring the sampletime with the oscilloscope

  while(micros() < next_loop_time) //is the current time stamp lower than the next fixed time to start the calculation loop? if yes-->wait so long until it is slightly bigger
  {
    //wait...
  }

  digitalWrite(USER_LED2, HIGH); // to start measuring the sampletime with the oscilloscope
  next_loop_time = next_loop_time + tsample_microseconds; //set the next fixed time so start the calculation loop (for the next iteration)

  /*Option 1: Reading in an value for the desired output voltage by Serial Monitor
  if(Serial.available() > 0)
  {
    float input_v_ref = Serial.parseFloat();
    if(input_v_ref >= 0.0 && input_v_ref <= 5.0)
    {
      V_ref = (double)input_v_ref;
      V_ref_filtered = V_ref*K;
      Serial.print("V_ref wurde auf ");
      Serial.print(input_v_ref);
      Serial.println(" aktualisiert.");
    }
  }*/

  //Option 2: Reading in an value for the desired output voltage by potentiometer
  potvolt = getAnalogVoltage(USER_POTI);
  V_ref_filtered = potvolt*K;
  

  Vout_meas = getAnalogVoltage(VOUT_MEAS);  //read in the measured voltage

  V_err = V_ref_filtered - Vout_meas;   //calculate the error
  current_integrator_sum = integrator_sum + V_err * tsample_seconds;  //add to our integrated sum until now the new iteration sum and store it in the current_integrator_sum
  dutycycle_command = (kP * V_err + kI * current_integrator_sum);   //calculate the duty cycle as a decimal, not as an ADC

  /*Anti-Windup*/
  if(dutycycle_command < 0.0)   //check if it is lower than 0
  {
    dutycycle = 0.0;  //then it should be 0
    saturation = true;  //and we hit the saturation
  }
  else if (dutycycle_command > 1.0)   //otherwise if its greater than 1
  {
    dutycycle = 1.0;    //then it should be 1
    saturation = true;  //we hit the saturation
  }
  else
  {
    dutycycle = dutycycle_command;
    saturation = false; //no saturation
  }

  if(saturation && V_err*dutycycle_command > 0.0) //if we are in saturation and the integrator wants to do it even worse f.e more dutycycle even if we are at a dutycycle of 100% (so same sign)
  {
    //dont update the integrator!
  }
  else  //if we are not in saturation and f.e error is negative --> lower dutycycle by adding an negative(Verr*tsampleseconds is negative) term --> dutycycle gets lower for the next time //analog: error positive
  {
    //update the integrator
    integrator_sum = current_integrator_sum; 
  }

  //PWM
  analogWrite(BUCK_CNTRL, adc_write_max*(1-dutycycle)); //write the dutycycle in arduino inverted, so the real PWM will be right
  
  end = micros(); //store current time stamp
  if(end > next_loop_time)  //if our current time is greater than the time we should start the next iteration, then light up the LED so we know our program is too slow
  {
    digitalWrite(USER_LED1, HIGH);
  }
  else  //otherwise let the LED be LOW
  {
    digitalWrite(USER_LED1, LOW);
  } //so now we know if the LED is blinking, that our Program is nearly as fast as our sample time
}
