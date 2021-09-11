#define FIRMWARE_VERSION "1.1.0"
#define HAS_STORAGE true
#define DEBOUNCE_MILLIS 5

#include <MPG.h>
#include <LUFA.h>
#include "LUFADriver.h"

uint32_t getMillis() { return millis(); }

MPG mpg(DEBOUNCE_MILLIS, HAS_STORAGE);

void setup()
{
	mpg.setup();
	mpg.load();
	mpg.read();

	InputMode inputMode = mpg.inputMode;
	if (mpg.pressedR3())
		inputMode = INPUT_MODE_HID;
	else if (mpg.pressedS1())
		inputMode = INPUT_MODE_SWITCH;
	else if (mpg.pressedS2())
		inputMode = INPUT_MODE_XINPUT;

	if (inputMode != mpg.inputMode)
	{
		mpg.inputMode = inputMode;
		mpg.save();
	}

	setupHardware(mpg.inputMode);
}

void loop()
{
	static const uint8_t reportSize = mpg.getReportSize();
	static void *report;

	mpg.read();
	mpg.debounce();
	mpg.hotkey();
	mpg.process();

	report = mpg.getReport();
	sendReport(report, reportSize);
}
