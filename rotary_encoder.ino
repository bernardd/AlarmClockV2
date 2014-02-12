/* rotary_encoder.ino
 * ~~~~~~~~~~~~~~~~~~
 * Please do not remove the following notices.
 * License: GPLv3. http://geekscape.org/static/arduino_license.html
 * ----------------------------------------------------------------------------
 * See http://www.circuitsathome.com/mcu/reading-rotary-encoder-on-arduino
 *
 * To Do
 * ~~~~~
 * - None, yet.
 */

int8_t inProgress = 0;

uint8_t rotaryEncoderValue = DEFAULT_LCD_BACKLIGHT;

void rotaryEncoderInitialize(void) {
  pinMode(PIN_ROTARY_A,    INPUT);
  pinMode(PIN_ROTARY_B,    INPUT);
  pinMode(PIN_ROTARY_PUSH, INPUT);

  digitalWrite(PIN_ROTARY_A,    HIGH);
  digitalWrite(PIN_ROTARY_B,    HIGH);
  digitalWrite(PIN_ROTARY_PUSH, HIGH);
}

void rotaryEncoderHandler(void) {
  int8_t delta = readRotaryEncoder();

  if (delta) {
    inProgress += delta;
    if (abs(inProgress) >= ROTARY_ENCODER_CLICK) {
      int8_t clicks = inProgress / ROTARY_ENCODER_CLICK;
      updateUI(clicks);
      inProgress %= ROTARY_ENCODER_CLICK;
    }
  }
  updateButton(!digitalRead(PIN_ROTARY_PUSH));
}

int8_t readRotaryEncoder(void) {
  static int8_t  encoderStates[] = {0, 1, -1, 0, -1, 0, 0, 1, 1, 0, 0, -1, 0, -1, 1, 0};
  static uint8_t encoderBits = 0;

  encoderBits <<= 2;
  encoderBits |= (ROTARY_ENCODER_PORT & ROTARY_ENCODER_MASK) >> 6;
  return(encoderStates[(encoderBits & 0x0f)]);
}

/* ------------------------------------------------------------------------- */
