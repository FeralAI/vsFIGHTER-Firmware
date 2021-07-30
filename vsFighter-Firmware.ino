#include <LUFA.h>
#include <EEPROM.h>
#include "UsbGamepad.h"
#include "VsFighter_Board.h"

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
	// Cache previous gamepad state
	GamepadState previousState = board.state;
	DpadMode previousDpadMode = dpadMode;

	// Read inputs
	board.update(dpadMode);

	// Check hotkeys
	HotkeyAction action = board.checkHotkeys();
	switch (action) {
		case HotkeyAction::DPAD_DIGITAL:
			dpadMode = DpadMode::DIGITAL;
			break;
		case HotkeyAction::DPAD_LEFT_ANALOG:
			dpadMode = DpadMode::LEFT_ANALOG;
			break;
		case HotkeyAction::DPAD_RIGHT_ANALOG:
			dpadMode = DpadMode::RIGHT_ANALOG;
			break;
	}

	// D-pad mode changed, persist to EEPROM and reprocess inputs
	if (previousDpadMode != dpadMode)
		EEPROM.put(EEPROM_DPAD_MODE_INDEX, dpadMode);

	// Process USB task for this loop cycle
	if (board.compareState(previousState))
		USB_USBTask();
	else
		sendGamepadState(board.state);
}
