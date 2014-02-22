/* ui.ino
 */

// UI


uiLine activeLine = Time;
byte activeCol = 0;
uiState currentState = Menu;
uiLine topDisplayLine = Banner;
byte lastButtonState = false;
uint32_t flashTimer;

void updateButton(byte pressed)
{
  if (pressed == lastButtonState) {
    return;
  } else if (pressed) {
    handlePress();
  }
  lastButtonState = pressed;
}

void updateUI(int8_t clicks)
{
  int8_t newLine;
  switch (currentState) {
    case Off:
      break;
    case Menu:
      newLine = activeLine + clicks;
      if (newLine < 0)
        newLine += NUM_MENU_LINES;
      activeLine = (uiLine)(newLine % NUM_MENU_LINES);
      
      if (activeLine-DISPLAY_LINES >= topDisplayLine)
        topDisplayLine = (uiLine)(activeLine-DISPLAY_LINES+1);
      else if (topDisplayLine+1 > activeLine)
        topDisplayLine = (uiLine)(activeLine-1);
        
      break;
    case Item:
      updateItem(clicks);
      break;
  }
}

void handlePress()
{
  switch (currentState) {
    case Off:
      currentState = Menu;
      lcdState(true);
      break;
    case Menu:
      switch (activeLine) {
        case Time:
        case Alarm:
        case Date:
        case Brightness:
          activeCol = 1;
          currentState = Item;
          break;
        case Screen:
          lcdState(false);
          activeLine = Time;
          activeCol = 0;
          topDisplayLine = Banner;
          currentState = Off;
          break;
      }
      break;
    case Item:
      switch (activeLine) {
        case Time:
        case Alarm:
        case Date:
          activeCol = (activeCol+1) % 4;
          if (activeCol == 0)
            currentState = Menu;
          break;
        case Brightness:
          currentState = Menu;
          activeCol = 0;
          break;
      }
  }
}

void adjust(int8_t clicks, uint16_t *n, int16_t start, int16_t max)
{
  int16_t tmp = *n + clicks;
  while (tmp < start)
    tmp += (max+1) - start;
    
  while (tmp > max)
    tmp -= (max+1) - start;
    
  *n = (uint16_t)tmp;
}

void adjustClock(int8_t clicks, hms *t)
{
  switch (activeCol) {
    case 1: adjust(clicks, &t->h, 0, 23); break;
    case 2: adjust(clicks, &t->m, 0, 59); break;
    case 3: adjust(clicks, &t->s, 0, 59); break;
  }
}

uint16_t daysInMonth(uint16_t month)
{
  switch (month) {
    case 2:
      if (RTC.is_leap_year(RTC.year))
        return 29;
      else
        return 28;
    case 4:
    case 6:
    case 9:
    case 11:
      return 30;
    default:
      return 31;
  }
}

void adjustDate(int8_t clicks, hms *t)
{
  uint16_t max;
  switch (activeCol) {
    case 1:
      max = daysInMonth(t->m);
      adjust(clicks, &t->h, 1, max); break;
    case 2: adjust(clicks, &t->m, 1, 12); break;
    case 3: adjust(clicks, &t->s, 2000, 2400); break;
  }
}

void updateItem(int8_t clicks)
{
  hms t;
  switch (activeLine) {
    case Time:
      loadTime(&t);
      adjustClock(clicks, &t);
      saveTime(&t);
      break;
    case Alarm:
      adjustClock(clicks, &alarm);
      saveAlarm();
      break;
    case Date:
      loadDate(&t);
      adjustDate(clicks, &t);
      saveDate(&t);
      break;
    case Brightness:
      backlightLevel = constrain(backlightLevel + clicks, 1, BACKLIGHT_STEPS);
      setBacklight();
      break;
  }
   

}

