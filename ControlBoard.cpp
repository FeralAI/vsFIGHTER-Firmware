#include "ControlBoard.h"

void ControlBoard::clearHotkeyState() {
	state.dpad = 0;
	state.buttons ^= GAMEPAD_BUTTON_09 | GAMEPAD_BUTTON_10;
}

bool ControlBoard::compareState(GamepadState otherState) {
	return state.dpad == otherState.dpad
		&& state.buttons == otherState.buttons
		&& state.lt == otherState.lt
		&& state.rt == otherState.rt
		&& state.lx == otherState.lx
		&& state.ly == otherState.ly
		&& state.rx == otherState.rx
		&& state.ry == otherState.ry;
}

uint16_t ControlBoard::dpadToAnalogX() {
	switch (state.dpad & (GAMEPAD_DPAD_LEFT | GAMEPAD_DPAD_RIGHT)) {
		case GAMEPAD_DPAD_LEFT:
			return GAMEPAD_AXIS_MIN;
		case GAMEPAD_DPAD_RIGHT:
			return GAMEPAD_AXIS_MAX;
		default:
			return GAMEPAD_AXIS_MID;
	}
}

uint16_t ControlBoard::dpadToAnalogY() {
	switch (state.dpad & (GAMEPAD_DPAD_UP | GAMEPAD_DPAD_DOWN)) {
		case GAMEPAD_DPAD_UP:
			return GAMEPAD_AXIS_MAX;
		case GAMEPAD_DPAD_DOWN:
			return GAMEPAD_AXIS_MIN;
		default:
			return GAMEPAD_AXIS_MID;
	}
}

bool ControlBoard::isDpadUpPressed() {
	return state.dpadInputs & GAMEPAD_DPAD_UP;
}

bool ControlBoard::isDpadDownPressed() {
	return state.dpadInputs & GAMEPAD_DPAD_DOWN;
}

bool ControlBoard::isDpadLeftPressed() {
	return state.dpadInputs & GAMEPAD_DPAD_LEFT;
}

bool ControlBoard::isDpadRightPressed() {
	return state.dpadInputs & GAMEPAD_DPAD_RIGHT;
}

bool ControlBoard::isHotkeyPressed() {
	return state.buttons & (GAMEPAD_BUTTON_09 | GAMEPAD_BUTTON_10);
}

bool ControlBoard::isSelectPressed() {
	return state.buttons & GAMEPAD_BUTTON_09;
}

bool ControlBoard::isStartPressed() {
	return state.buttons & GAMEPAD_BUTTON_10;
}

void ControlBoard::pressHomeButton() {
	state.buttons |= GAMEPAD_BUTTON_13;
}