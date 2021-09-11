#define FIRMWARE_VERSION "1.0.2"
#define DEBOUNCE_MILLIS 5

#include <LUFA.h>
#include "USB_HID.h"
#include "VsFighter.h"

VsFighter board(DEBOUNCE_MILLIS, true);

uint32_t getMillis() { return millis(); }


void setup() {
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

	if (inputMode != board.inputMode)
	{
		board.inputMode = inputMode;
		board.save();
	}

	SetupHardware(board.inputMode);
}


void loop() {
	static const uint16_t reportSize = board.getReportSize();
	static void *report;

	board.read();
	board.debounce();
	board.hotkey();
	board.process();
	report = board.getReport();

	sendReport(report, reportSize);
}
