#include <Arduino.h>
#include <Servo.h>

const int LED_PIN = 2;
const int SERVO_PIN = 9;
const int FLEX_PIN = A0;
float angleServo;
long tempsMs;
float heure;
long cpt1 = 0, cpt2 = 0;
Servo servoMoteur;
int oui=5000;
int non = 0;
void setup(){
	Serial.begin(9600);
	pinMode(LED_PIN,OUTPUT);
	pinMode(LED_PIN,OUTPUT);
	pinMode(FLEX_PIN,INPUT);
	pinMode(13, OUTPUT);
	servoMoteur.attach(SERVO_PIN);  
	//attachInterrupt(0, resetFunc, FALLING);
}


void clignoterLed(){
	unsigned long temps = millis();
	if(temps-cpt2 >=1000){
		cpt2=temps;
		digitalWrite(13,HIGH);
		delay(5);
		digitalWrite(13,LOW);
	}
}

void loop(){
	clignoterLed();
	int flex = analogRead(FLEX_PIN);
	if(flex < oui)
		oui = flex;
	Serial.println("Valeur " + String(flex));
	if(flex < 718){
		servoMoteur.write(0);
		digitalWrite(LED_PIN, HIGH);
	}
	delay(100);
}