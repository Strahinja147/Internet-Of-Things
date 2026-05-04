#include "DHT.h"
#include <Servo.h>
#include <EEPROM.h>
#include <LiquidCrystal.h>

#define DHTPIN 8
#define DHTTYPE DHT22
#define BUTTON_PIN 19 
#define SERVO_PIN 9


DHT dht(DHTPIN, DHTTYPE,4);
Servo myservo;


LiquidCrystal lcd(1, 2, 4, 5, 6, 7);


#define EEPROM_HUMIDITY_ADDR 0
#define EEPROM_TEMP_ADDR     1

volatile bool sistem_Active = false;
volatile int counter = 0;
volatile bool merenje_Flag = false;

ISR(TIMER2_COMPA_vect) {
	counter++;
	if (counter >= 1500) {
		merenje_Flag = true;
		counter = 0;
	}
}



void prekidTaster() {
	sistem_Active = !sistem_Active;
	lcd.clear();
	if (!sistem_Active) {
		myservo.write(0); 
		lcd.print("Sistem iskljucen");
	} 
	else {
		lcd.print("Sistem aktivan");
	}
}




void setup() {
	
	dht.begin();
	myservo.attach(SERVO_PIN);

	lcd.begin(16, 2); 
	lcd.print("Pokretanje...");

	pinMode(BUTTON_PIN, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), prekidTaster, FALLING);

	EEPROM.write(0, 60); 
	EEPROM.write(1, 28); 


  
	cli(); 

	TCCR2A = (1 << WGM21);           
	TCCR2B = (1 << CS22);              
	OCR2A = 249;                       
	TIMSK2 |= (1 << OCIE2A);           

	sei(); 

	//lcd.clear();
}

void loop() {

	float v = dht.readHumidity();
	float t = dht.readTemperature();

	if (sistem_Active && merenje_Flag) {
		merenje_Flag = false;
  
		if (isnan(v) || isnan(t) ) {
			lcd.clear();
			lcd.print("Greska senzora!");
			return;
		}


		byte granicnaVlaznost = EEPROM.read(EEPROM_HUMIDITY_ADDR);
		byte granicnaTemperatura = EEPROM.read(EEPROM_TEMP_ADDR);

		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("T:");
		lcd.print(t, 1);
		lcd.print("C H:");
		lcd.print(v, 0);
		lcd.print("%");

		if (v > granicnaVlaznost || t > granicnaTemperatura) {
			myservo.write(180); 
			lcd.setCursor(0, 1);
			lcd.print("Prozor: otvoren ");
		} 
		else {
			myservo.write(0);   
			lcd.setCursor(0, 1);
			lcd.print("Prozor: zatvoren");
		}
	}
}
