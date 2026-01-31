#define VIN_MEAS A3  //Pin for reading in the measured input voltage

void setup()
{
  pinMode(VIN_MEAS, INPUT);  //define the pin as an input
  Serial.begin(9600);  //open the serial monitor with baud rate 9600

  analogReadResolution(12);  //set the ADC-reading resolution to 12 bit
}

//initializing
int adc_value = 0;
double adc_voltage = 0, input_voltage = 0;

void loop()
{
  adc_value = analogRead(VIN_MEAS);
  adc_voltage = adc_value*(5.0/4095.0);  //transform the read in adc value into an adc voltage
  input_voltage = adc_voltage*(4.7+22)/4.7;  //and also transform it to the real input voltage by using the voltage divider formula
  //Printing all the values
  Serial.print("Input-Voltage: ");
  Serial.print(input_voltage);

  Serial.print(", ADC-Voltage: ");
  Serial.print(adc_voltage);

  Serial.print(", ADC-Value: ");
  Serial.println(adc_value);

  delay(100);
}
