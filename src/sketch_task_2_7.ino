#define USER_LED1 9
#define USER_LED2 10
#define USER_LED3 11
#define USER_LED4 12
#define BUCK_CNTRL 6

int delaytime = 500;

void setup()
{
pinMode(USER_LED1, OUTPUT);
pinMode(USER_LED2, OUTPUT);
pinMode(USER_LED3, OUTPUT);
pinMode(USER_LED4, OUTPUT);
pinMode(BUCK_CNTRL, OUTPUT);

//Initialisieren
digitalWrite(USER_LED1, LOW);
digitalWrite(USER_LED2, LOW);
digitalWrite(USER_LED3, LOW);
digitalWrite(USER_LED4, LOW);
digitalWrite(BUCK_CNTRL, LOW);

//Serielle Schnittstelle
Serial.begin(9600);
Serial.print("Ganzzahlige Frequenz, mit der die LED1 blinken soll: ");
}

int frequency_hz = 0;

void loop()
{
  if(Serial.available() > 0)
  {
    frequency_hz = Serial.parseInt();

    if(frequency_hz > 0)
    {
      //Ausgabe zur Bestätigung für den Nutzer
      Serial.print("Eingegebene Frequenz: ");
      Serial.println(frequency_hz);
      delaytime = (1000/frequency_hz)/2;
      Serial.print("Delaytime: ");
      Serial.println(delaytime);
    }
  }

  digitalWrite(USER_LED1, HIGH);
  delay(delaytime);
  digitalWrite(USER_LED1, LOW);
  delay(delaytime);
}
