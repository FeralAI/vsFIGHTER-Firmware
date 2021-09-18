#include "LEDController.h"

volatile uint16_t blinkDelay = BLINK_DELAY;
volatile uint8_t remainingBlinks = 0;
volatile bool ledOn = true;
volatile uint16_t nextBlinkTime = 0;
CRGB lastStatusColor;

void blinkStatusLED(uint8_t count, uint16_t delayMillis = BLINK_DELAY)
{
	blinkDelay = delayMillis;
	remainingBlinks = count;
}

void configureLEDs()
{
	pinMode(STATUS_LED_PIN, OUTPUT);
	FastLED.addLeds<WS2812, STATUS_LED_PIN, GRB>(statusLEDs, 1);
	FastLED.setBrightness(LED_BRIGHTNESS);
}

void setStatusLEDColor(CRGB color)
{
	if (!LedEnabled)
		return;

	statusLEDs[0] = color;
	FastLED.show();
}

void tryBlinkStatusLED(CEveryNMilliseconds *ledTimer)
{
	if (!LedEnabled)
		return;

	uint32_t currentTime = millis();
	if (remainingBlinks > 0 && currentTime > nextBlinkTime)
	{
		if (ledOn)
		{
			lastStatusColor = statusLEDs[0];
			setStatusLEDColor(CRGB::Black);
			ledOn = false;
		}
		else
		{
			--remainingBlinks;
			setStatusLEDColor(lastStatusColor);
			ledOn = true;
		}

		nextBlinkTime = currentTime + blinkDelay;
	}
}
