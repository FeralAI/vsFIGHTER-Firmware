#include "Gamepad.h"

Gamepad::Gamepad(uint32_t debounceMicros) {
	this->debounceMicros = debounceMicros;
}

HotkeyAction Gamepad::checkHotkeys() {
	HotkeyAction action = HotkeyAction::NONE;
	if (isFunctionPressed()) {
		if (isDpadLeftPressed()) {
			action = HotkeyAction::DPAD_LEFT_ANALOG;
			state.dpad = 0;
			state.buttons &= ~(GAMEPAD_BUTTON_09 | GAMEPAD_BUTTON_10);
		} else if (isDpadRightPressed()) {
			action = HotkeyAction::DPAD_RIGHT_ANALOG;
			state.dpad = 0;
			state.buttons &= ~(GAMEPAD_BUTTON_09 | GAMEPAD_BUTTON_10);
		} else if (isDpadDownPressed()) {
			action = HotkeyAction::DPAD_DIGITAL;
			state.dpad = 0;
			state.buttons &= ~(GAMEPAD_BUTTON_09 | GAMEPAD_BUTTON_10);
		} else if (isDpadUpPressed()) {
			action = HotkeyAction::HOME_BUTTON;
			state.dpad = 0;
			state.buttons |= GAMEPAD_BUTTON_13;
			state.buttons &= ~(GAMEPAD_BUTTON_09 | GAMEPAD_BUTTON_10);
		}
	}

	return action;
}

bool Gamepad::checkStateChanged(GamepadState otherState) {
	return state.dpad == otherState.dpad
	    && state.buttons == otherState.buttons
	    && state.lt == otherState.lt
	    && state.rt == otherState.rt
	    && state.lx == otherState.lx
	    && state.ly == otherState.ly
	    && state.rx == otherState.rx
	    && state.ry == otherState.ry;
}

uint16_t Gamepad::dpadToAnalogX() {
	switch (state.dpad & (GAMEPAD_DPAD_LEFT | GAMEPAD_DPAD_RIGHT)) {
		case GAMEPAD_DPAD_LEFT:
			return GAMEPAD_AXIS_MIN;
		case GAMEPAD_DPAD_RIGHT:
			return GAMEPAD_AXIS_MAX;
		default:
			return GAMEPAD_AXIS_MID;
	}
}

uint16_t Gamepad::dpadToAnalogY() {
	switch (state.dpad & (GAMEPAD_DPAD_UP | GAMEPAD_DPAD_DOWN)) {
		case GAMEPAD_DPAD_UP:
			return GAMEPAD_AXIS_MAX;
		case GAMEPAD_DPAD_DOWN:
			return GAMEPAD_AXIS_MIN;
		default:
			return GAMEPAD_AXIS_MID;
	}
}

void Gamepad::debounce() {
	if (debounceMicros == 0)
		return;

	// Get the difference between last and this input
	uint32_t currentTime = micros();

	// DEBOUNCE!
	for (int i = 0; i < GAMEPAD_ACTIVE_BUTTON_COUNT; i++) {
		// Did the button change state and are we past the timer?
		if ((state.buttonInputs & GAMEPAD_BUTTON_MASKS[i]) != (previousState.buttonInputs & GAMEPAD_BUTTON_MASKS[i])) {
			if (currentTime - buttonDebounceTimers[i] > debounceMicros) {
				// Button state change allowed, track current time
				buttonDebounceTimers[i] = currentTime;
			} else {
				// Toggle bit to revert state
				state.buttonInputs ^= GAMEPAD_BUTTON_MASKS[i];
			}
		}
	}

	// Do the same for the dpad
	for (int i = 0; i < 4; i++) {
		if ((state.dpadInputs & GAMEPAD_DPAD_MASKS[i]) != (previousState.dpadInputs & GAMEPAD_DPAD_MASKS[i])) {
			if (currentTime - buttonDebounceTimers[i] > debounceMicros) {
				dpadDebouceTimers[i] = currentTime;
			} else {
				state.dpadInputs ^= GAMEPAD_DPAD_MASKS[i];
			}
		}
	}
}

SwitchInputReport Gamepad::getSwitchReport() {
	SwitchInputReport report;

	// Direct assignments
	report.LX = state.lx >> 8;
	report.LY = state.ly >> 8;
	report.RX = state.rx >> 8;
	report.RY = state.ry >> 8;

	// Handle HAT switch (D-Pad)
	switch (state.dpad & (GAMEPAD_DPAD_UP | GAMEPAD_DPAD_DOWN | GAMEPAD_DPAD_LEFT | GAMEPAD_DPAD_RIGHT)) {
		case GAMEPAD_DPAD_UP:                        report.HAT = SWITCH_DPAD_VALUE_UP;        break;
		case GAMEPAD_DPAD_UP | GAMEPAD_DPAD_RIGHT:   report.HAT = SWITCH_DPAD_VALUE_UPRIGHT;   break;
		case GAMEPAD_DPAD_RIGHT:                     report.HAT = SWITCH_DPAD_VALUE_RIGHT;     break;
		case GAMEPAD_DPAD_DOWN | GAMEPAD_DPAD_RIGHT: report.HAT = SWITCH_DPAD_VALUE_DOWNRIGHT; break;
		case GAMEPAD_DPAD_DOWN:                      report.HAT = SWITCH_DPAD_VALUE_DOWN;      break;
		case GAMEPAD_DPAD_DOWN | GAMEPAD_DPAD_LEFT:  report.HAT = SWITCH_DPAD_VALUE_DOWNLEFT;  break;
		case GAMEPAD_DPAD_LEFT:                      report.HAT = SWITCH_DPAD_VALUE_LEFT;      break;
		case GAMEPAD_DPAD_UP | GAMEPAD_DPAD_LEFT:    report.HAT = SWITCH_DPAD_VALUE_UPLEFT;    break;
		default:                                     report.HAT = SWITCH_DPAD_VALUE_NOTHING;   break;
	}

	// Convert button states
	report.Button = 0
		| ((state.buttons & GAMEPAD_BUTTON_03) ? SWITCH_MASK_Y       : 0) // Y
		| ((state.buttons & GAMEPAD_BUTTON_01) ? SWITCH_MASK_B       : 0) // B
		| ((state.buttons & GAMEPAD_BUTTON_02) ? SWITCH_MASK_A       : 0) // A
		| ((state.buttons & GAMEPAD_BUTTON_04) ? SWITCH_MASK_X       : 0) // X
		| ((state.buttons & GAMEPAD_BUTTON_05) ? SWITCH_MASK_L       : 0) // L
		| ((state.buttons & GAMEPAD_BUTTON_06) ? SWITCH_MASK_R       : 0) // R
		| ((state.buttons & GAMEPAD_BUTTON_07) ? SWITCH_MASK_ZL      : 0) // ZL
		| ((state.buttons & GAMEPAD_BUTTON_08) ? SWITCH_MASK_ZR      : 0) // ZR
		| ((state.buttons & GAMEPAD_BUTTON_09) ? SWITCH_MASK_MINUS   : 0) // -
		| ((state.buttons & GAMEPAD_BUTTON_10) ? SWITCH_MASK_PLUS    : 0) // +
		| ((state.buttons & GAMEPAD_BUTTON_11) ? SWITCH_MASK_L3      : 0) // Left Stick Click
		| ((state.buttons & GAMEPAD_BUTTON_12) ? SWITCH_MASK_R3      : 0) // Right Stick Click
		| ((state.buttons & GAMEPAD_BUTTON_13) ? SWITCH_MASK_HOME    : 0) // Home
		| ((state.buttons & GAMEPAD_BUTTON_14) ? SWITCH_MASK_CAPTURE : 0) // Capture
	;

	return report;
}

XInputReport Gamepad::getXInputReport() {
	XInputReport report;
	report.rsize = 20;

	// Direct assignments
	report.l_x = state.lx + -32768;
	report.l_y = state.ly + -32768;
	report.r_x = state.rx + -32768;
	report.r_y = state.ry + -32768;

	// Convert button states
	report.digital_buttons_1 = state.dpad                                 // Dpad values, same as GamepadState
		| ((state.buttons & GAMEPAD_BUTTON_10) ? XBOX_MASK_START       : 0) // Start/Menu
		| ((state.buttons & GAMEPAD_BUTTON_09) ? XBOX_MASK_BACK        : 0) // Back/View
		| ((state.buttons & GAMEPAD_BUTTON_11) ? XBOX_MASK_LEFT_STICK  : 0) // Left Stick Click
		| ((state.buttons & GAMEPAD_BUTTON_12) ? XBOX_MASK_RIGHT_STICK : 0) // Right Stick Click
	;

	report.digital_buttons_2 = 0
		| ((state.buttons & GAMEPAD_BUTTON_05) ? XBOX_MASK_LB : 0)   // Left Bumper
		| ((state.buttons & GAMEPAD_BUTTON_06) ? XBOX_MASK_RB : 0)   // Right Bumper
		| ((state.buttons & GAMEPAD_BUTTON_13) ? XBOX_MASK_HOME : 0) // Home
		// unused
		| ((state.buttons & GAMEPAD_BUTTON_01) ? XBOX_MASK_A : 0)    // A
		| ((state.buttons & GAMEPAD_BUTTON_02) ? XBOX_MASK_B : 0)    // B
		| ((state.buttons & GAMEPAD_BUTTON_03) ? XBOX_MASK_X : 0)    // X
		| ((state.buttons & GAMEPAD_BUTTON_04) ? XBOX_MASK_Y : 0)    // Y
	;

	// Handle trigger values
	if (state.hasAnalogTriggers) {
		report.lt = state.lt;                                         // Left Trigger (analog)
		report.rt = state.rt;                                         // Right Trigger (analog)
	} else {
		report.lt = (state.buttons & GAMEPAD_BUTTON_07) ? 0xFFFF : 0; // Left Trigger (digital)
		report.rt = (state.buttons & GAMEPAD_BUTTON_08) ? 0xFFFF : 0; // Right Trigger (digital)
	}

	return report;
}

bool Gamepad::isDpadUpPressed() {
	return state.dpadInputs & GAMEPAD_DPAD_UP;
}

bool Gamepad::isDpadDownPressed() {
	return state.dpadInputs & GAMEPAD_DPAD_DOWN;
}

bool Gamepad::isDpadLeftPressed() {
	return state.dpadInputs & GAMEPAD_DPAD_LEFT;
}

bool Gamepad::isDpadRightPressed() {
	return state.dpadInputs & GAMEPAD_DPAD_RIGHT;
}

bool Gamepad::isFunctionPressed() {
	return state.buttons & (GAMEPAD_BUTTON_09 | GAMEPAD_BUTTON_10);
}

bool Gamepad::isLeftStickButtonPressed() {
	return state.buttons & GAMEPAD_BUTTON_11;
}

bool Gamepad::isRightStickButtonPressed() {
	return state.buttons & GAMEPAD_BUTTON_12;
}

bool Gamepad::isSelectPressed() {
	return state.buttons & GAMEPAD_BUTTON_09;
}

bool Gamepad::isStartPressed() {
	return state.buttons & GAMEPAD_BUTTON_10;
}

void Gamepad::read() { }

uint8_t Gamepad::runSOCD(bool up, bool down, bool left, bool right) {
	if (up && down)    { down = false; }
	if (left && right) { left = false; right = false; }

	return 0
		^ (up ? GAMEPAD_DPAD_UP : 0)
		^ (down ? GAMEPAD_DPAD_DOWN : 0)
		^ (left ? GAMEPAD_DPAD_LEFT : 0)
		^ (right ? GAMEPAD_DPAD_RIGHT : 0)
	;
}

void Gamepad::setup() { }

void Gamepad::update() {
	if (checkStateChanged(previousState)) {
		// No state change, just maintain USB connection
		USB_USBTask();
	} else {
		// Gamepad state changed, send inputs to host
		switch (inputMode) {
			case InputMode::XINPUT:
				sendXInputReport(&getXInputReport());
				break;
			case InputMode::SWITCH:
				sendSwitchReport(&getSwitchReport());
				break;
		}
	}

	previousState = state;
}
