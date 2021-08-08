#include "GamepadDebouncer.h"

bool GamepadDebouncer::readCurrentState() {
	if (isDpad)
		return (*gamepadState).dpadInputs & inputMask;
	else
		return (*gamepadState).buttonInputs & inputMask;
}
