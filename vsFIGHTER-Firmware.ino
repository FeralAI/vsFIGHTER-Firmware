/**
 * vsFIGHTER-Firmware
 * v2.0.0
 * 
 * Multiplatform gamepad firmware supporting XInput, DirectInput and Nintendo Switch.
 */

#define DEBOUNCE_MILLIS 5

#define REV_V3B

#include <LUFA.h>
#include "LUFADriver.h"
#include "BoardSelect.h"
#include "VsFighterStorage.h"
#ifdef HAS_STATUS_LED
#include "LEDController.h"
#endif

VsFighterStorage storage;
VsFighter board(DEBOUNCE_MILLIS, &storage);

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

#ifdef HAS_STATUS_LED
	if (board.pressedA1())
		storage.setDisabledStatusLED(!storage.getDisableStatusLED());

	LedEnabled = !storage.getDisableStatusLED();
	if (LedEnabled)
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

	if (GamepadDebouncer::getMillis() - nextRuntime < 0)
		return;

	board.read();
#if DEBOUNCE_MILLIS > 0
	board.debounce();
#endif
	hotkey = board.hotkey();

#ifdef HAS_STATUS_LED
	if (hotkey != GamepadHotkey::HOTKEY_NONE)
		blinkStatusLED(3);

	EVERY_N_MILLIS_I(thisTimer, 50)
	{
		tryBlinkStatusLED(&thisTimer);
	}
#endif

	board.process();

	report = board.getReport();
	sendReport(report, reportSize);

	nextRuntime = GamepadDebouncer::getMillis() + intervalMS;
}
