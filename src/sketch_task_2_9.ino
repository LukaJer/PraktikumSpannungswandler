#define BUCK_CNTRL 6
#define USER_BUTTON1 3
#define USER_BUTTON2 4
#define VIN_MEAS A3
#define VOUT_MEAS A4
#define NTC_EXT A5

void setup() {
  Serial.begin(9600);
  analogReadResolution(12);

  pinMode(BUCK_CNTRL, OUTPUT);
  digitalWrite(BUCK_CNTRL, LOW);

}


const float R_series = 10000; //Series Resistance in Ohm
const float R_rated = 10000; //Reference/Rated Resistance in Ohm
const float T_rated = 298.15; //Reference/Rated Temperature in Kelvin
const float B = 3380; //K
const float adc_res = 4095; //ADC-Resolution
const float V_R = 5.0; //Reference Voltage

const int PWM_frequency = 150000; //Hz;

int adcvalue = 0;
float V_NTC = 0, T= 0;

void loop() {
  
  adcvalue = analogRead(NTC_EXT);
  V_NTC = (adcvalue*5.0)/adc_res;

  T = (B*T_rated)/(T_rated*log(R_series/(((V_R/V_NTC)-1.0)*R_rated))+B);  //deriven formula for the Temperature
  T = T - 273.15; //convert from Kelvin to Celsius

  Serial.print("ADC-Value: ");
  Serial.print(adcvalue);
  Serial.print(", NTC-Voltage: ");
  Serial.print(V_NTC);
  Serial.print(", NTC-Temperature: ");
  Serial.print(T);
  Serial.println(" °C");

  setAnalogWriteFrequency(BUCK_CNTRL, PWM_frequency); //PWM signal to gerate some output voltage
  analogWrite(BUCK_CNTRL, 0.6* getAnalogWriteMaximum());

  delay(1000);

}
