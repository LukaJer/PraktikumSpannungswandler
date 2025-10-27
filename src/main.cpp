#include <Arduino.h>

// === Analog Pins ===
#define PIN_IN_CURR_ADC        17
#define PIN_INDUCTOR_CURR_ADC  18
#define PIN_USER_POTI          19
#define PIN_VIN_MEAS           20
#define PIN_VOUT_MEAS          21
#define PIN_NTC_EXT            22

// === Digital Pins ===
#define PIN_USER_BTN1          9
#define PIN_USER_BTN2          10
#define PIN_BUCK_CNTRL_ARDUINO 12
#define PIN_SHUTDOWN           13
#define PIN_USER_LED1          4
#define PIN_USER_LED2          5
#define PIN_USER_LED3          6
#define PIN_USER_LED4          7
#define PIN_USER_SW            8

// === Temperature sensor parameters (adjust as needed) ===
const float VREF = 5;              // reference voltage (e.g. 3.3V or 5.0V)
const int ADC_RES = 4095;            // 12-bit ADC
const float SERIES_RESISTOR = 10000; // resistor in series with NTC (10k typical)
const float NOMINAL_RESISTANCE = 10000; // NTC resistance at 25°C
const float NOMINAL_TEMPERATURE = 25.0; // °C
const float B_COEFFICIENT = 3380.0;     // Beta constant for the thermistor

// === Function declaration ===
float readTemperatureC();

void setup() {
  Serial.begin(9600);
  delay(500);
  Serial.println("Temperature reading started...\n");
}

void loop() {
  float temperature = readTemperatureC();

  Serial.print("Temperature: ");
  Serial.print(temperature, 2);
  Serial.println(" °C");

  delay(1000);
}

// === Function definition ===
float readTemperatureC() {
  int adcValue = analogRead(PIN_NTC_EXT);
  
  // avoid divide-by-zero if voltage reads 0
  if (adcValue == 0) return NAN;

  // Convert ADC reading to voltage
  float voltage = (adcValue * VREF) / ADC_RES;

  // Convert voltage to resistance (voltage divider)
  float resistance = SERIES_RESISTOR * (VREF / voltage - 1.0);

  // Apply Steinhart–Hart approximation
  float steinhart;
  steinhart = resistance / NOMINAL_RESISTANCE;     
  steinhart = log(steinhart);                      
  steinhart /= B_COEFFICIENT;                      
  steinhart += 1.0 / (NOMINAL_TEMPERATURE + 273.15);
  steinhart = 1.0 / steinhart;                     
  steinhart -= 273.15;                             

  return steinhart;
}
