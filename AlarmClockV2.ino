#include <OneWire.h>

#include <AikoCommands.h>
//#include <AikoEvents.h>
#include <Wire.h>
#include <DS1307new.h>

#define LED_PIN 11

#include "aiko_pebble_v2.h"

void setup(void) {
  Serial.begin(DEFAULT_BAUD_RATE);
  
  rotaryEncoderInitialize();
  lcdInitialise();
  lcdClear();
  
  loadAlarm();

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  printFreeRam();
}

void loop(void) {
  RTC.getTime();
  lcdHandler();
}

/* ------------------------------------------------------------------------- */
// http://jeelabs.org/2011/05/22/atmega-memory-use

int freeRam(void) {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

void printFreeRam () {
  Serial.print("Free RAM: ");
  Serial.println(freeRam());
}

/* ------------------------------------------------------------------------- */
