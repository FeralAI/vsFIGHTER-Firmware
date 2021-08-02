#include <EEPROM.h>
#include <LUFA.h>
#include "USB_HID.h"

// Set DEBOUNCE_MICROS to 0 to disable debouncing
#define DEBOUNCE_MICROS 5000

#include "VsFighter_Board.h"
VsFighterBoard board(DEBOUNCE_MICROS);


void setup() {
	// Get saved options
	EEPROM.get(EEPROM_INPUT_MODE_INDEX, board.inputMode);
	EEPROM.get(EEPROM_DPAD_MODE_INDEX, board.dpadMode);
	EEPROM.get(EEPROM_LED_MODE_INDEX, board.ledMode);

	// Configure board
	board.setup();
	delay(500);

	// Perform initial button read
	board.update();

	// Set input mode - only write to EEPROM if necessary
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

			// D-pad mode changed, persist to EEPROM
			if (lastDpadMode != board.dpadMode)
				EEPROM.put(EEPROM_DPAD_MODE_INDEX, board.dpadMode);
		}
	}

	// Process USB task for this loop cycle
	board.update();
}
