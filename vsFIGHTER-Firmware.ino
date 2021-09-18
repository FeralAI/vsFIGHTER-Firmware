/**
 * vsFIGHTER-Firmware
 * v2.0.0
 * 
 * Multiplatform gamepad firmware supporting XInput, DirectInput and Nintendo Switch.
 */

#define DEBOUNCE_MILLIS 5

#include <LUFA.h>
#include "LUFADriver.h"
#include "BoardSelect.h"
#include "VsFighterStorage.h"
#ifdef STATUS_LED_PIN
#include "LEDController.h"
bool ledDisabled = false;
#endif

VsFighterStorage storage;
VsFighter board(DEBOUNCE_MILLIS, &storage);

uint32_t getMillis() { return millis(); }

void setup()
{
	board.setup();
	board.load();
	board.read();

	InputMode inputMode = board.inputMode;
	if (board.pressedR3())
		inputMode = INPUT_MODE_HID;
	else if (board.pressedS1())
		inputMode = INPUT_MODE_SWITCH;
	else if (board.pressedS2())
		inputMode = INPUT_MODE_XINPUT;

#ifdef STATUS_LED_PIN
	ledDisabled = storage.getDisableStatusLED();
	if (board.pressedA1())
	{
		ledDisabled = !ledDisabled;
		storage.setDisableStatusLED(ledDisabled);
	}

	if (!ledDisabled)
	{
		configureLEDs();

		switch (inputMode)
		{
			case INPUT_MODE_HID:
				setStatusLEDColor(HIDLEDColor);
				break;
			case INPUT_MODE_SWITCH:
				setStatusLEDColor(SwitchLEDColor);
				break;
			case INPUT_MODE_XINPUT:
				setStatusLEDColor(XInputLEDColor);
				break;
		}
	}
#endif

	if (inputMode != board.inputMode)
	{
		board.inputMode = inputMode;
		board.save();
	}

	setupHardware(board.inputMode);
}

void loop()
{
	static const uint8_t reportSize = board.getReportSize();
	static void *report;
	static GamepadHotkey hotkey;
	static const uint32_t intervalMS = 1;
	static uint32_t nextRuntime = 0;

	if (millis() - nextRuntime < 0)
		return;

	board.read();
#if DEBOUNCE_MILLIS > 0
	board.debounce();
#endif
	hotkey = board.hotkey();

	board.process();

	report = board.getReport();
	sendReport(report, reportSize);

#ifdef STATUS_LED_PIN
	if (!ledDisabled)
	{
		if (hotkey != GamepadHotkey::HOTKEY_NONE)
			blinkStatusLED(3);

		EVERY_N_MILLIS_I(thisTimer, 50)
		{
			tryBlinkStatusLED(&thisTimer);
		}
	}
#endif

	nextRuntime = millis() + intervalMS;
}
