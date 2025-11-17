void setup()
{
  double start = 0, end = 0, duration = 0;
  Serial.begin(9600);
  delay(1000);
  start = millis();
  for(int i = 0; i < 4; i++)
  {
    Serial.println("Hello World");
  }
  end = millis();
  duration = end - start;
  delay(1000);
  Serial.print("Dauer: ");
  Serial.print(duration);
}

void loop()
{

}
