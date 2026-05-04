#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <Arduino.h>
#include <EEPROM.h>


#define POT A0
#define RELAY_PIN 15
#define BUTTON_PIN 18
#define EEPROM_ADDR 0

volatile bool systemActive = false;
float voltage1 = 0, voltage2 = 0;
float voltageThreshold;

SemaphoreHandle_t First, Second, Measure;


void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), toggleSystem, RISING);

  Serial.begin(9600);


  EEPROM.write(0, 1); 


  digitalWrite(RELAY_PIN, HIGH); 

  First = xSemaphoreCreateBinary();
  Second = xSemaphoreCreateBinary();
  Measure = xSemaphoreCreateBinary();

  xTaskCreate(measureTask, "MeasureTask", 128, NULL, 1, NULL);
  xTaskCreate(firstMeasureTask, "firstMeasureTask", 128, NULL, 1, NULL);
  xTaskCreate(secondMeasureTask, "secondMeasureTask", 128, NULL, 1, NULL);

  xSemaphoreGive(First);
}


void loop()
{
}


void toggleSystem() {
  systemActive = !systemActive;
  if (systemActive) {
    voltageThreshold=EEPROM.read(EEPROM_ADDR);
  } else {
    digitalWrite(RELAY_PIN, HIGH); 
  }
}

void firstMeasureTask(void *pvParameters)
{
	for(;;){
		if(xSemaphoreTake(First, portMAX_DELAY) && systemActive){
			voltage1 = analogRead(POT) * (5.0 / 1023.0);
			vTaskDelay(5000 / portTICK_PERIOD_MS);
		}
		xSemaphoreGive(Second);
	}
}

void secondMeasureTask(void *pvParameters)
{
	for(;;){
		if(xSemaphoreTake(Second, portMAX_DELAY) && systemActive){
			voltage2 = analogRead(POT) * (5.0 / 1023.0);
			//vTaskDelay(1000 / portTICK_PERIOD_MS);
		}
		xSemaphoreGive(Measure);
	}
}


void measureTask(void *pvParameters) {
  for (;;) {
    if (xSemaphoreTake(Measure, portMAX_DELAY) && systemActive) {  
      float diff = fabs(voltage1 - voltage2);

      if (diff > voltageThreshold) {
        digitalWrite(RELAY_PIN, LOW); 
      } 
	  else {
        digitalWrite(RELAY_PIN, HIGH); 
      }

      vTaskDelay(1000 / portTICK_PERIOD_MS);
    } 
	else {
      vTaskDelay(500 / portTICK_PERIOD_MS); 
    }
	xSemaphoreGive(First);
  }
}
