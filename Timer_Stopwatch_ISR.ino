
#include <LiquidCrystal.h>
#include <avr/interrupt.h>

LiquidCrystal lcd(11, 10, 5, 4, 3, 2); // RS=11, E=10, D4=5, D5=4, D6=3, D7=2

volatile unsigned int sekunde = 0;
volatile unsigned int minuti = 0;
volatile bool aktivna = false;
volatile unsigned int brojac_ms = 0;

void prikaziVreme() {
  lcd.setCursor(0, 1);

  if (minuti < 10) lcd.print("0");
  lcd.print(minuti);
  lcd.print(":");
  if (sekunde < 10) lcd.print("0");
  lcd.print(sekunde);

  lcd.print("      ");
}

void prekidStartPause() {
  aktivna = !aktivna;
}

void prekidReset() {
  aktivna = false;
  sekunde = 0;
  minuti = 0;
  brojac_ms = 0;
}

void setup() {
  
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Stopwatch:");
  prikaziVreme();

  pinMode(7, INPUT);
  pinMode(6, INPUT);

  attachInterrupt(digitalPinToInterrupt(7), prekidStartPause, RISING);
  attachInterrupt(digitalPinToInterrupt(6), prekidReset, RISING);

  cli(); 

  TCCR0A = (1 << WGM01);                
  TCCR0B = (1 << CS01) | (1 << CS00);    
  OCR0A = 249;                          
  TIMSK0 |= (1 << OCIE0A);               

  sei(); 
}

ISR(TIMER0_COMPA_vect) {
  if (aktivna) {
    brojac_ms++;
    if (brojac_ms >= 1000) {
      brojac_ms = 0;
      sekunde++;
      if (sekunde >= 60) {
        sekunde = 0;
        minuti++;
        if (minuti >= 60) {
          minuti = 60;
          aktivna = false; 
        }
      }
    }
  }
}

void loop() {
  static unsigned int staraSek = 99;
  static unsigned int staraMin = 99;

  if (sekunde != staraSek || minuti != staraMin) {
    staraSek = sekunde;
    staraMin = minuti;
    prikaziVreme();
  }

  delay(50); 
}
