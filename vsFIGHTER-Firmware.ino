/**
 * @brief vsFIGHTER-Firmware
 * @version 2.0.0
 * 
 * Multiplatform gamepad firmware supporting XInput, DirectInput and Nintendo Switch.
 */

#define DEBOUNCE_MILLIS 5

/* Uncomment **ONE** of the following defines to build for a board revision */
// #define REV_V2B
#define REV_V2B

#include <LUFA.h>
#include "LUFADriver.h"
#include "LEDController.h"

#include "VsFighterStorage.h"
#ifdef REV_V3B
#include "Boards/VsFighter_v3b.h"
#endif

#ifdef REV_V2B
#include "Boards/VsFighter_v2b.h"
#endif

#ifdef REV_V2
#include "Boards/VsFighter.h"
#endif

VsFighterStorage storage;
VsFighter board(DEBOUNCE_MILLIS, &storage);

void setup()
{
	board.setup();
	board.load();
	board.read();

	if (board.pressedA1())
		storage.setDisabledStatusLED(!storage.getDisableStatusLED());

	LedEnabled = !storage.getDisableStatusLED();

	InputMode inputMode = board.inputMode;
	if (board.pressedR3())
		inputMode = INPUT_MODE_HID;
	else if (board.pressedS1())
		inputMode = INPUT_MODE_SWITCH;
	else if (board.pressedS2())
		inputMode = INPUT_MODE_XINPUT;

	if (inputMode != board.inputMode)
	{
		board.inputMode = inputMode;
		board.save();
	}

	if (LedEnabled)
	{
		configureLEDs();

		switch (board.inputMode)
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

	setupHardware(board.inputMode);
}

void loop()
{
	EVERY_N_MILLIS_I(thisTimer, 50)
	{
		tryBlinkStatusLED(&thisTimer);
	}

	static const uint8_t reportSize = board.getReportSize();
	static void *report;

	board.read();
#if DEBOUNCE_MILLIS > 0
	board.debounce();
#endif
	GamepadHotkey hotkey = board.hotkey();
	if (hotkey != GamepadHotkey::HOTKEY_NONE)
		blinkStatusLED(3);

	board.process();

	report = board.getReport();
	sendReport(report, reportSize);
}
