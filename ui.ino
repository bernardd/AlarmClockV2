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

void adjust(int8_t clicks, uint16_t *n, uint16_t wrapAt)
{
  int16_t tmp = *n + clicks;
  while (tmp < 0)
    tmp += wrapAt-1;
    
  while (tmp >= wrapAt)
    tmp -= wrapAt;
    
  *n = tmp;
}

void adjustClock(int8_t clicks, hms *t)
{
  switch (activeCol) {
    case 1: adjust(clicks, &t->h, 24); break;
    case 2: adjust(clicks, &t->m, 60); break;
    case 3: adjust(clicks, &t->s, 60); break;
  }
}

void updateItem(int8_t clicks)
{
  switch (activeLine) {
    case Time:
      hms t;
      loadTime(&t);
      adjustClock(clicks, &t);
      saveTime(&t);
      break;
    case Alarm:
      adjustClock(clicks, &alarm);
      saveAlarm();
      break;
  }
   

}

