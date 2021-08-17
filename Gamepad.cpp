#include "Gamepad.h"

void Gamepad::configureInputMode() {
	read();

	InputMode lastInputMode = inputMode;
	if (isRightStickButtonPressed()) inputMode = InputMode::DUALSHOCK3;
	else if (isSelectPressed())      inputMode = InputMode::SWITCH;
	else if (isStartPressed())       inputMode = InputMode::XINPUT;
	if (lastInputMode != inputMode)
		Storage.setInputMode(inputMode);
}

void Gamepad::debounce() {
	for (int i = 0; i < GAMEPAD_DEBOUNCE_BUTTON_COUNT; i++) {
		// Did we detect a change?
		if (debouncers[i].update()) {
			// Was it a press?
			if (debouncers[i].rose()) {
				// Dpad or buttons?
				if (debouncers[i].isDpad)
					currentState.dpadInputs |= debouncers[i].inputMask;
				else
					currentState.buttonInputs |= debouncers[i].inputMask;
			} else {
				if (debouncers[i].isDpad)
					currentState.dpadInputs &= ~(debouncers[i].inputMask);
				else
					currentState.buttonInputs &= ~(debouncers[i].inputMask);
			}
		}
	}
}

DS3Report Gamepad::getDS3Report() {
	DS3Report report;
	report.reportId = 1;

	// Direct assignments
	report.lx = currentState.lx >> 8;
	report.ly = currentState.ly >> 8;
	report.rx = currentState.rx >> 8;
	report.ry = currentState.ry >> 8;

	report.buttons = 0
		| ((currentState.buttons & GAMEPAD_BUTTON_09)  ? DS3_BUTTON_SELECT_MASK   : 0)
		| ((currentState.buttons & GAMEPAD_BUTTON_11)  ? DS3_BUTTON_L3_MASK       : 0)
		| ((currentState.buttons & GAMEPAD_BUTTON_12)  ? DS3_BUTTON_R3_MASK       : 0)
		| ((currentState.buttons & GAMEPAD_BUTTON_10)  ? DS3_BUTTON_START_MASK    : 0)
		| ((currentState.dpad    & GAMEPAD_DPAD_UP)    ? DS3_DPAD_UP_MASK         : 0)
		| ((currentState.dpad    & GAMEPAD_DPAD_RIGHT) ? DS3_DPAD_RIGHT_MASK      : 0)
		| ((currentState.dpad    & GAMEPAD_DPAD_DOWN)  ? DS3_DPAD_DOWN_MASK       : 0)
		| ((currentState.dpad    & GAMEPAD_DPAD_LEFT)  ? DS3_DPAD_LEFT_MASK       : 0)
		| ((currentState.buttons & GAMEPAD_BUTTON_07)  ? DS3_BUTTON_L2_MASK       : 0)
		| ((currentState.buttons & GAMEPAD_BUTTON_08)  ? DS3_BUTTON_R2_MASK       : 0)
		| ((currentState.buttons & GAMEPAD_BUTTON_05)  ? DS3_BUTTON_L1_MASK       : 0)
		| ((currentState.buttons & GAMEPAD_BUTTON_06)  ? DS3_BUTTON_R2_MASK       : 0)
		| ((currentState.buttons & GAMEPAD_BUTTON_04)  ? DS3_BUTTON_TRIANGLE_MASK : 0)
		| ((currentState.buttons & GAMEPAD_BUTTON_02)  ? DS3_BUTTON_CIRCLE_MASK   : 0)
		| ((currentState.buttons & GAMEPAD_BUTTON_01)  ? DS3_BUTTON_CROSS_MASK    : 0)
		| ((currentState.buttons & GAMEPAD_BUTTON_03)  ? DS3_BUTTON_SQUARE_MASK   : 0)
		| ((currentState.buttons & GAMEPAD_BUTTON_13)  ? DS3_BUTTON_PS_MASK       : 0)
	;

	return report;
}

SwitchInputReport Gamepad::getSwitchReport() {
	SwitchInputReport report;

	// Direct assignments
	report.LX = currentState.lx >> 8;
	report.LY = currentState.ly >> 8;
	report.RX = currentState.rx >> 8;
	report.RY = currentState.ry >> 8;

	// Handle HAT switch (D-Pad)
	switch (currentState.dpad & (GAMEPAD_DPAD_UP | GAMEPAD_DPAD_DOWN | GAMEPAD_DPAD_LEFT | GAMEPAD_DPAD_RIGHT)) {
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
		| ((currentState.buttons & GAMEPAD_BUTTON_03) ? SWITCH_MASK_Y       : 0) // Y
		| ((currentState.buttons & GAMEPAD_BUTTON_01) ? SWITCH_MASK_B       : 0) // B
		| ((currentState.buttons & GAMEPAD_BUTTON_02) ? SWITCH_MASK_A       : 0) // A
		| ((currentState.buttons & GAMEPAD_BUTTON_04) ? SWITCH_MASK_X       : 0) // X
		| ((currentState.buttons & GAMEPAD_BUTTON_05) ? SWITCH_MASK_L       : 0) // L
		| ((currentState.buttons & GAMEPAD_BUTTON_06) ? SWITCH_MASK_R       : 0) // R
		| ((currentState.buttons & GAMEPAD_BUTTON_07) ? SWITCH_MASK_ZL      : 0) // ZL
		| ((currentState.buttons & GAMEPAD_BUTTON_08) ? SWITCH_MASK_ZR      : 0) // ZR
		| ((currentState.buttons & GAMEPAD_BUTTON_09) ? SWITCH_MASK_MINUS   : 0) // -
		| ((currentState.buttons & GAMEPAD_BUTTON_10) ? SWITCH_MASK_PLUS    : 0) // +
		| ((currentState.buttons & GAMEPAD_BUTTON_11) ? SWITCH_MASK_L3      : 0) // Left Stick Click
		| ((currentState.buttons & GAMEPAD_BUTTON_12) ? SWITCH_MASK_R3      : 0) // Right Stick Click
		| ((currentState.buttons & GAMEPAD_BUTTON_13) ? SWITCH_MASK_HOME    : 0) // Home
		| ((currentState.buttons & GAMEPAD_BUTTON_14) ? SWITCH_MASK_CAPTURE : 0) // Capture
	;

	return report;
}

XInputReport Gamepad::getXInputReport() {
	XInputReport report;
	report.rsize = 20;

	// Direct assignments
	report.l_x = currentState.lx + -32768;
	report.l_y = currentState.ly + -32768;
	report.r_x = currentState.rx + -32768;
	report.r_y = currentState.ry + -32768;

	// Convert button states
	report.digital_buttons_1 = currentState.dpad                                 // Dpad values, same as GamepadState
		| ((currentState.buttons & GAMEPAD_BUTTON_10) ? XBOX_MASK_START       : 0) // Start/Menu
		| ((currentState.buttons & GAMEPAD_BUTTON_09) ? XBOX_MASK_BACK        : 0) // Back/View
		| ((currentState.buttons & GAMEPAD_BUTTON_11) ? XBOX_MASK_LEFT_STICK  : 0) // Left Stick Click
		| ((currentState.buttons & GAMEPAD_BUTTON_12) ? XBOX_MASK_RIGHT_STICK : 0) // Right Stick Click
	;

	report.digital_buttons_2 = 0
		| ((currentState.buttons & GAMEPAD_BUTTON_05) ? XBOX_MASK_LB : 0)   // Left Bumper
		| ((currentState.buttons & GAMEPAD_BUTTON_06) ? XBOX_MASK_RB : 0)   // Right Bumper
		| ((currentState.buttons & GAMEPAD_BUTTON_13) ? XBOX_MASK_HOME : 0) // Home
		// unused
		| ((currentState.buttons & GAMEPAD_BUTTON_01) ? XBOX_MASK_A : 0)    // A
		| ((currentState.buttons & GAMEPAD_BUTTON_02) ? XBOX_MASK_B : 0)    // B
		| ((currentState.buttons & GAMEPAD_BUTTON_03) ? XBOX_MASK_X : 0)    // X
		| ((currentState.buttons & GAMEPAD_BUTTON_04) ? XBOX_MASK_Y : 0)    // Y
	;

	// Handle trigger values
	if (hasAnalogTriggers) {
		report.lt = currentState.lt;                                         // Left Trigger (analog)
		report.rt = currentState.rt;                                         // Right Trigger (analog)
	} else {
		report.lt = (currentState.buttons & GAMEPAD_BUTTON_07) ? 0xFFFF : 0; // Left Trigger (digital)
		report.rt = (currentState.buttons & GAMEPAD_BUTTON_08) ? 0xFFFF : 0; // Right Trigger (digital)
	}

	return report;
}

HotkeyAction Gamepad::hotkey() {
	HotkeyAction action = HotkeyAction::NONE;
	if (isDpadHotkeyPressed()) {
		DpadMode lastDpadMode = dpadMode;
		if (isDpadLeftPressed()) {
			action = HotkeyAction::DPAD_LEFT_ANALOG;
			dpadMode = DpadMode::LEFT_ANALOG;
			currentState.buttonInputs &= ~(GAMEPAD_BUTTON_09 | GAMEPAD_BUTTON_10);
			currentState.dpadInputs = 0;
		} else if (isDpadRightPressed()) {
			action = HotkeyAction::DPAD_RIGHT_ANALOG;
			dpadMode = DpadMode::RIGHT_ANALOG;
			currentState.buttonInputs &= ~(GAMEPAD_BUTTON_09 | GAMEPAD_BUTTON_10);
			currentState.dpadInputs = 0;
		} else if (isDpadDownPressed()) {
			action = HotkeyAction::DPAD_DIGITAL;
			dpadMode = DpadMode::DIGITAL;
			currentState.buttonInputs &= ~(GAMEPAD_BUTTON_09 | GAMEPAD_BUTTON_10);
			currentState.dpadInputs = 0;
		} else if (isDpadUpPressed()) {
			action = HotkeyAction::HOME_BUTTON;
			currentState.dpadInputs = 0;
			currentState.buttonInputs &= ~(GAMEPAD_BUTTON_09 | GAMEPAD_BUTTON_10);
			currentState.buttonInputs |= GAMEPAD_BUTTON_13; // Press the Home button
		}

		if (lastDpadMode != dpadMode)
			Storage.setDpadMode(dpadMode);

	} else if (isSOCDHotkeyPressed()) {
		SOCDMode lastSOCDMode = socdMode;
		if (isDpadUpPressed()) {
			action = HotkeyAction::SOCD_HITBOX;
			socdMode = SOCDMode::HITBOX;
			currentState.dpadInputs = 0;
			currentState.buttonInputs &= ~(GAMEPAD_BUTTON_11 | GAMEPAD_BUTTON_12);
		} else if (isDpadDownPressed()) {
			action = HotkeyAction::SOCD_NEUTRAL;
			socdMode = SOCDMode::NEUTRAL;
			currentState.dpadInputs = 0;
			currentState.buttonInputs &= ~(GAMEPAD_BUTTON_11 | GAMEPAD_BUTTON_12);
		} else if (isDpadLeftPressed()) {
			action = HotkeyAction::SOCD_LAST_INPUT;
			socdMode = SOCDMode::LAST_INPUT;
			currentState.dpadInputs = 0;
			currentState.buttonInputs &= ~(GAMEPAD_BUTTON_11 | GAMEPAD_BUTTON_12);
		}

		if (lastSOCDMode != socdMode)
			Storage.setSOCDMode(socdMode);

	}

	return action;
}

bool Gamepad::isDpadHotkeyPressed() {
	return (currentState.buttonInputs & (GAMEPAD_BUTTON_09 | GAMEPAD_BUTTON_10)) == (GAMEPAD_BUTTON_09 | GAMEPAD_BUTTON_10);
}

bool Gamepad::isSOCDHotkeyPressed() {
	return (currentState.buttonInputs & (GAMEPAD_BUTTON_11 | GAMEPAD_BUTTON_12)) == (GAMEPAD_BUTTON_11 | GAMEPAD_BUTTON_12);
}

void Gamepad::load() {
	dpadMode  = Storage.getDpadMode();
	inputMode = Storage.getInputMode();
	socdMode  = Storage.getSOCDMode();
}

void Gamepad::process() {
	// Set button states
	currentState.buttons = currentState.buttonInputs;
	currentState.dpad = runSOCD(socdMode, currentState.dpadInputs);

	switch (dpadMode) {
		case DpadMode::LEFT_ANALOG:
			currentState.lx = dpadToAnalogX(currentState.dpad);
			currentState.ly = dpadToAnalogY(currentState.dpad);
			if (!hasRightAnalogStick) {
				currentState.rx = GAMEPAD_AXIS_MID;
				currentState.ry = GAMEPAD_AXIS_MID;
			}
			currentState.dpad = 0;
			break;
		case DpadMode::RIGHT_ANALOG:
			if (!hasLeftAnalogStick) {
				currentState.lx = GAMEPAD_AXIS_MID;
				currentState.ly = GAMEPAD_AXIS_MID;
			}
			currentState.rx = dpadToAnalogX(currentState.dpad);
			currentState.ry = dpadToAnalogY(currentState.dpad);
			currentState.dpad = 0;
			break;
		default:
			if (!hasLeftAnalogStick) {
				currentState.lx = GAMEPAD_AXIS_MID;
				currentState.ly = GAMEPAD_AXIS_MID;
			}
			if (!hasRightAnalogStick) {
				currentState.rx = GAMEPAD_AXIS_MID;
				currentState.ry = GAMEPAD_AXIS_MID;
			}
			break;
	}
}

void Gamepad::read() { }

void Gamepad::setup() { }

void Gamepad::update() {
	switch (inputMode) {
		case InputMode::XINPUT:
			sendReport(&getXInputReport(), sizeof(XInputReport));
			break;
		case InputMode::SWITCH:
			sendReport(&getSwitchReport(), sizeof(SwitchInputReport));
			break;
	}

	memcpy(&previousState, &currentState, sizeof(currentState));
}
