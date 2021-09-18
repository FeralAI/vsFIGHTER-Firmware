#ifndef LED_CONTROLLER_H_
#define LED_CONTROLLER_H_

#include <FastLED.h>

#define BLINK_DELAY 125
#define LED_BRIGHTNESS 64 // 8-bit value (0-255)

#ifndef STATUS_LED_PIN
#define STATUS_LED_PIN 13 // PC7
#endif

static CRGB statusLEDs[1];

static const CRGB HIDLEDColor = CRGB::DeepSkyBlue;
static const CRGB SwitchLEDColor = CRGB::FireBrick;
static const CRGB XInputLEDColor = CRGB::LimeGreen;

void blinkStatusLED(uint8_t count, uint16_t delayMillis = BLINK_DELAY);
void configureLEDs();
void setStatusLEDColor(CRGB color);
void tryBlinkStatusLED(CEveryNMilliseconds *ledTimer);

#endif
