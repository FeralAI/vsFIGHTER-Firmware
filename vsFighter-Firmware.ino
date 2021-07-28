#include <LUFA.h>
#include <EEPROM.h>
#include "UsbGamepad.h"
#include "Controllers/VsFighter/VsFighter_Board.h"

#define EEPROM_INPUT_MODE_INDEX 0
#define EEPROM_DPAD_MODE_INDEX  1
#define EEPROM_LED_MODE_INDEX   2

VsFighterBoard board;                    // The board instance
InputMode inputMode = InputMode::XINPUT; // The input mode, default to XINPUT
DpadMode dpadMode = DpadMode::DIGITAL;   // Current D-Pad mode
uint8_t ledMode = 0;                     // The selected LED mode

void setup() {
	// Get saved options
	EEPROM.get(EEPROM_INPUT_MODE_INDEX, inputMode);
	EEPROM.get(EEPROM_DPAD_MODE_INDEX, dpadMode);
	EEPROM.get(EEPROM_LED_MODE_INDEX, ledMode);

	// Configure board
	board.setupPins();
	delay(500);

	// Set input mode
	board.update(dpadMode);
	bool isSwitch = board.isSelectPressed();
	bool isXInput = board.isStartPressed();
	if (isSwitch && !isXInput) {
		inputMode = InputMode::SWITCH;
		EEPROM.put(EEPROM_INPUT_MODE_INDEX, inputMode);
	} else if (isXInput && !isSwitch) {
		inputMode = InputMode::XINPUT;
		EEPROM.put(EEPROM_INPUT_MODE_INDEX, inputMode);
	}

	// Configure USB HID
	SetupHardware(inputMode);
	GlobalInterruptEnable();
}

void loop() {
	// Read inputs
	board.update(dpadMode);

	// Check hotkeys
	if (board.isHotkeyPressed()) {
		bool update = true;
		bool pressed = false;
		if (board.isDpadLeftPressed()) {
			dpadMode = LEFT_ANALOG;
			pressed = true;
		} else if (board.isDpadRightPressed()) {
			dpadMode = RIGHT_ANALOG;
			pressed = true;
		} else if (board.isDpadUpPressed()) {
			dpadMode = DIGITAL;
			pressed = true;
		} else if (board.isDpadDownPressed()) {
			board.pressHomeButton();
			update = false;
			pressed = true;
		}
		else update = false;

		// D-pad mode changed, persist to EEPROM and reprocess inputs
		if (update) {
			EEPROM.put(EEPROM_DPAD_MODE_INDEX, dpadMode);
			board.update(dpadMode);
		}

		// Clear hotkey presses after any state updates
		if (pressed)
			board.clearHotkeyState();
	}

	// Send button state
	sendGamepadState(board.state);
}
