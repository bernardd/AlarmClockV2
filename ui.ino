/* ui.ino
 */

// UI


uiLine activeLine = Time;
byte activeCol = 0;
uiState currentState = Off;
uiLine topDisplayLine = Banner;
byte lastButtonState = false;
uint32_t flashTimer;

void updateButton(byte clicked)
{
  if (clicked == lastButtonState) {
    return;
  } else if (!clicked) {
    lastButtonState = false;
    return;
  } else {
    lastButtonState = true;
    handleClick();
  }
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
        topDisplayLine = (uiLine)(activeLine-DISPLAY_LINES);
      else if (topDisplayLine+1 > activeLine)
        topDisplayLine = (uiLine)(activeLine+1);
        
      break;
    case Item:
      updateItem(clicks);
      break;
  }
}

void handleClick()
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
          topDisplayLine = Time;
          currentState = Off;
          break;
      }
      break;
    case Item:
      switch (activeLine) {
        case Time:
        case Alarm:
          if (activeCol == 0) {
            currentState = Menu;
            activeCol = 0;
          } else {
            activeCol = (activeCol+1) % 3;
          }
          break;
        case Brightness:
          currentState = Menu;
          activeCol = 0;
          break;
      }
  }
}

void updateItem(uint8_t clicks)
{
  
}