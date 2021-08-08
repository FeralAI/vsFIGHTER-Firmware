#include <LUFA.h>

// Override default debounce time set for the board, 0 to disable
#define DEBOUNCE_MILLIS 5

#include "VsFighter.h"
VsFighter board;

void setup() {
	// Configure board
	board.setup();

	// Get saved options
	board.load();

	// Set input mode and persist if necessary
	board.configureInputMode();

	// Configure USB HID
	SetupHardware(board.inputMode);
	GlobalInterruptEnable();
}


void loop() {
	// Read inputs
	board.read();

#if DEBOUNCE_MILLIS > 0
	// Run debouncing if required
	board.debounce();
#endif

	// Check for hotkey changes, can react to returned hotkey action
	board.hotkey();

	// Process the raw inputs into a usable state
	board.process();

	// Run the USB task for this loop cycle
	board.update();
}
