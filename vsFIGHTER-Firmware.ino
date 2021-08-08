#include <EEPROM.h>
#include <LUFA.h>

// Override default debounce time set for the board, 0 to disable
#define DEBOUNCE_MILLIS 5

#include "VsFighter.h"
VsFighter board;

void setup() {
	// Get saved options
	EEPROM.get(EEPROM_INPUT_MODE_INDEX, board.inputMode);
	EEPROM.get(EEPROM_DPAD_MODE_INDEX, board.dpadMode);
	EEPROM.get(EEPROM_LED_MODE_INDEX, board.ledMode);

	// Configure board
	board.setup();

	// Perform initial button read
	board.read();

	// Set input mode and persist if necessary
	InputMode lastInputMode = board.inputMode;
	if (board.isRightStickButtonPressed()) board.inputMode = InputMode::DUALSHOCK3;
	else if (board.isSelectPressed())      board.inputMode = InputMode::SWITCH;
	else if (board.isStartPressed())       board.inputMode = InputMode::XINPUT;
	if (lastInputMode != board.inputMode)
		EEPROM.put(EEPROM_INPUT_MODE_INDEX, board.inputMode);

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

	// Check hotkeys
	HotkeyAction action = board.checkHotkeys();
	if (action != HotkeyAction::NONE) {
		// Has Dpad mode changed?
		if (action & (HotkeyAction::DPAD_DIGITAL | HotkeyAction::DPAD_LEFT_ANALOG | HotkeyAction::DPAD_RIGHT_ANALOG)) {
			DpadMode lastDpadMode = board.dpadMode;
			switch (action) {
				case HotkeyAction::DPAD_DIGITAL:      board.dpadMode = DpadMode::DIGITAL;      break;
				case HotkeyAction::DPAD_LEFT_ANALOG:  board.dpadMode = DpadMode::LEFT_ANALOG;  break;
				case HotkeyAction::DPAD_RIGHT_ANALOG: board.dpadMode = DpadMode::RIGHT_ANALOG; break;
			}

			// Persist dpad mode change
			if (lastDpadMode != board.dpadMode)
				EEPROM.put(EEPROM_DPAD_MODE_INDEX, board.dpadMode);
		}
	}

	// Process the raw inputs into a usable state
	board.process();

	// Run the USB task for this loop cycle
	board.update();
}
