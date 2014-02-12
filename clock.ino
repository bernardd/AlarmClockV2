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
  RTC.fillByHMS(t->h, t->m, t->s);
  RTC.setTime();
  RTC.startClock();
  RTC.getTime();
}

void loadDate(hms *t)
{
  t->h = RTC.day;
  t->m = RTC.month;
  t->s = RTC.year;
}

void saveDate(hms *t)
{
  RTC.fillByYMD(t->s, t->m, t->h);
  RTC.setTime();
  RTC.startClock();
  RTC.getTime();
}

uint32_t hmsInSec(hms *t)
{
  return t->h * 3600 + t->m * 60 + t->s;
}

byte checkAlarm()
{
  hms t;
  loadTime(&t);
  int32_t secs = hmsInSec(&t);
  int32_t aSecs = hmsInSec(&alarm);
  int32_t delta = secs - aSecs;
  
  while (delta < 0)
    delta += (3600*24);
  
  if (delta < 3600)
    return true;
  else
    return false;
}
