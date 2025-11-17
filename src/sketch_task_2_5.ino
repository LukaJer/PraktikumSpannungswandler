#define USER_LED1 9
#define USER_LED2 10
#define USER_LED3 11
#define USER_LED4 12
#define BUCK_CNTRL 6
#define USER_BUTTON1 3

int button1state = 0;
int currentledstate = 0;

void setup() {

pinMode(USER_LED1, OUTPUT);
pinMode(USER_LED2, OUTPUT);
pinMode(USER_LED3, OUTPUT);
pinMode(USER_LED4, OUTPUT);
pinMode(BUCK_CNTRL, OUTPUT);

pinMode(USER_BUTTON1, INPUT);

//Initialisieren
digitalWrite(USER_LED1, LOW);
digitalWrite(USER_LED2, LOW);
digitalWrite(USER_LED3, LOW);
digitalWrite(USER_LED4, LOW);
digitalWrite(BUCK_CNTRL, LOW);
}

//Exercise 1
/*void loop() 
{
  button1state = digitalRead(USER_BUTTON1);
  if(button1state)
  {
    digitalWrite(USER_LED1, HIGH);
  }
  else
  {
    digitalWrite(USER_LED1, LOW);
  }
}*/


//Exercise 2
void loop()
{
  button1state = digitalRead(USER_BUTTON1);
  if(button1state && !currentledstate)
  {
    digitalWrite(USER_LED1, HIGH);
    currentledstate = 1;
    delay(300);
  }
  else if(button1state && currentledstate)
  {
    digitalWrite(USER_LED1, LOW);
    currentledstate = 0;
    delay(300);
  }
}
