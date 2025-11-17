#define VIN_MEAS A3

void setup()
{
  pinMode(VIN_MEAS, INPUT);
  Serial.begin(9600);

  analogReadResolution(12);
}

int adc_value = 0;
double adc_voltage = 0, input_voltage = 0;

void loop()
{
  adc_value = analogRead(VIN_MEAS);
  adc_voltage = adc_value*(5.0/4095.0);
  input_voltage = adc_voltage*(4.7+22)/4.7;

  Serial.print("Input-Voltage: ");
  Serial.print(input_voltage);

  Serial.print(", ADC-Voltage: ");
  Serial.print(adc_voltage);

  Serial.print(", ADC-Value: ");
  Serial.println(adc_value);

  delay(100);
}
