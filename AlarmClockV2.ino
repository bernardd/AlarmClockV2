#include <OneWire.h>

#include <AikoCommands.h>
#include <AikoEvents.h>

using namespace Aiko;
using namespace Command;

#include "aiko_pebble_v2.h"

void setup(void) {
  Serial.begin(DEFAULT_BAUD_RATE);

  Events.addHandler(rotaryEncoderHandler,    1);  // Aim for < 1 ms
  Events.addHandler(lcdHandler,           1000);  // Aim for 1000 ms
// Uses expensive floating point - leave out for now:
//  Events.addHandler(temperatureHandler,   1000);  // Aim for 250 ms

  printFreeRam();
}

void loop(void) {
  Events.loop();
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
