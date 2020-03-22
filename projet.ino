#include <Arduino.h>
#include <Servo.h>
#include <LiquidCrystal.h>


// declaration des pins des entrées/sorties
const int LED_PIN = 2;
const int SERVO_PIN = 9;
const int PHOTOCELL_PIN = A0;
const int BOUTON_PIN = 3;

unsigned int tempsOn = 50;
unsigned int tempsOff = 950;
unsigned long tempsMemo = 0;
boolean commut = true;
boolean courrier = false; // indique la présence de courrier (true si présent, false sinon)
Servo servoMoteur;
unsigned long tempsMs1;
unsigned long tempsMs2 = 0;
int heures;
int minutes;
LiquidCrystal lcd(12, 11, 7, 6, 5, 4);


void initHorloge(){
	Serial.println("Saisir heures (hh) : ");
	//heures = Serial.read();
	heures = 18;
	Serial.println("Saisir minutes (mm) : ");
	//minutes = Serial.read();
	minutes = 40;
}


void initLCD() {
	lcd.clear();
	lcd.begin(16,2);
}

void calculHeure(){
	tempsMs1 = millis();
	if((tempsMs1 - tempsMs2) >= 60000){
		tempsMs2 = tempsMs1;
		minutes ++;
	}
	if(minutes >= 60){
		heures ++;
		minutes -= 60;
	}
	if(heures >= 24)
		heures = 0;
}

/*
	fonction : clignoterLed
	description : fait clignoter la led de la carte toutes les secondes pendant 50ms en utilisant la fonction millis();
*/
void clignoterLed(){
	unsigned long temps = millis();
	if(temps >= tempsMemo){
		digitalWrite(13, commut); //inverse l'etat de la led
		if(commut)
			tempsMemo += tempsOn;
		else
			tempsMemo += tempsOff;
		commut = !commut;
	}
}

void leverFanion(){
	servoMoteur.write(180); //baisse le fanion (servomoteur à 180°)
}

void baisserFanion(){
	servoMoteur.write(0); //baisse le fanion (servomoteur à 0°)
}

void allumerLed(){
	digitalWrite(LED_PIN, HIGH); //allume la led
}

void eteindreLed(){
	digitalWrite(LED_PIN, LOW); //éteint la led
}

void afficherHeureCourante(){

}

void afficherHeurePassage(){
	
}

/*
	fonction : verifierCourrier
	description : vérifie la présence de courrier grâce au capteur de lumière, retourne true s'il y a du courrier, false sinon
*/
boolean verifierCourrier(){
	int valeurPhotocell = analogRead(PHOTOCELL_PIN);
	if(valeurPhotocell < 800){
		Serial.println("Passage : " + String(heures) + "h" + String(minutes));
		leverFanion();
		allumerLed();
		return true;
	}
	return false;
}

void reinitialiser(){
	courrier = false;
	baisserFanion();
	eteindreLed();
}

void setup(){
	Serial.begin(9600);

	pinMode(LED_PIN,OUTPUT);
	pinMode(PHOTOCELL_PIN,INPUT);
	pinMode(13, OUTPUT);

	servoMoteur.attach(SERVO_PIN);  

	attachInterrupt(BOUTON_PIN, reinitialiser, FALLING);

	initHorloge();
	initLCD();
}

void loop(){
	clignoterLed();
	calculHeure();
	if(!courrier)
		courrier = verifierCourrier();
	Serial.println(String(heures) + "h" + String(minutes));
}	


