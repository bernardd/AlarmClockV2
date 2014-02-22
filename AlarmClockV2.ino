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

byte alarmState = 0;

void loop(void) {
  RTC.getTime();
  lcdHandler();
  byte alarmOn = checkAlarm();

  if (alarmOn && !alarmState)
    digitalWrite(LED_PIN, HIGH);
  else if (!alarmOn && alarmState)
    digitalWrite(LED_PIN, LOW);

  alarmState = alarmOn;
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


char * getDayEvent()
{
  switch (RTC.month) {
    case 1: // Jan
      switch (RTC.day) {
        case 1: return "Happy new year!";
        case 6: return "Epiphany";
        case 26: return "Australia Day";
        case 28: return "Dallas gets NFL team";
        default: return defaultEvent();
      }
    case 2: // Feb
      switch (RTC.day) {
        case 2: return "Groundhog Day";
        case 4: return "Rosa Parks Day";
        case 12: return "Lincoln's Birthday";
        case 14: return "Valentine's Day";
        case 15: return "Susan B. Anthony Day";
        default: return defaultEvent();
      }
    case 3: // Mar
      switch (RTC.day) {
        case 17: return "St Patrick's Day";
        default: return defaultEvent();
      }
    case 4: // Apr
      switch (RTC.day) {
        case 16: return "Emancipation Day";
        case 22: return "Happy BDay Mandy!";
        case 25: return "ANZAC Day";
        default: return defaultEvent();
      }
    case 5: // May
      switch (RTC.day) {
        case 5: return "Cinco de Mayo";
        default: return defaultEvent();
      }
    case 6: // June
      switch (RTC.day) {
        case 3: return "Happy BDay Oscar!";
        case 27: return "Helen Keller Day";
        default: return defaultEvent();
      }
    case 7: // July
      switch (RTC.day) {
        case 4: return "Independence Day";
        default: return defaultEvent();
      }
    case 8: // Aug
      switch (RTC.day) {
        case 26: return "Women's Equality Day";
        default: return defaultEvent();
      }
    case 9: // Sep
      switch (RTC.day) {
        default: return defaultEvent();
      }
    case 10: // Oct
      switch (RTC.day) {
        case 31: return "Haloween - BOO!";
        default: return defaultEvent();
      }
    case 11: // Nov
      switch (RTC.day) {
        case 25: return "Happy Birthday Fi!";
        default: return defaultEvent();
      }
    case 12: // Dec
      switch (RTC.day) {
        case 16: return "Jane Austen's BDay";
        case 24: return "Christmas Eve";
        case 25: return "Merry Christmas!";
        case 26: return "Boxing Day";
        case 31: return "New Year's Eve";
        default: return defaultEvent();
      }
    
  }

}

char * defaultEvent()
{
  return "OscarClock v1";
}
