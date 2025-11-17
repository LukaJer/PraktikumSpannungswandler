#define USER_LED1 9
#define USER_LED2 10
#define USER_LED3 11
#define USER_LED4 12
#define BUCK_CNTRL 6

void setup() {
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
}

void loop() {
digitalWrite(USER_LED1, HIGH);
delay(250);
digitalWrite(USER_LED1, LOW);
delay(250);
digitalWrite(USER_LED2, HIGH);
delay(250);
digitalWrite(USER_LED2, LOW);
delay(250);
digitalWrite(USER_LED3, HIGH);
delay(250);
digitalWrite(USER_LED3, LOW);
delay(250);
digitalWrite(USER_LED4, HIGH);
delay(250);
digitalWrite(USER_LED4, LOW);
delay(250);
}
