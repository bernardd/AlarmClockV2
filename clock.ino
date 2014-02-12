/* clock.ino

Various clock functions
*/

#include <Wire.h>
#include <DS1307new.h>
#include "aiko_pebble_v2.h"

hms alarm;

void saveAlarm()
{
  RTC.setRAM(0, (uint8_t *)&alarm, sizeof(alarm));
}

void loadAlarm()
{
  RTC.getRAM(0, (uint8_t *)&alarm, sizeof(alarm));
}

void loadTime(hms *t)
{
  t->h = RTC.hour;
  t->m = RTC.minute;
  t->s = RTC.second;
}

void saveTime(hms *t)
{
  RTC.stopClock();
  RTC.hour = t->h;
  RTC.minute = t->m;
  RTC.second = t->s;
  RTC.setTime();
  RTC.startClock();
}

byte checkAlarm()
{
  return false;
}
