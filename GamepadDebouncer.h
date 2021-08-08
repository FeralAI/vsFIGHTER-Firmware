#ifndef _GAMEPAD_DEBOUNCER_
#define _GAMEPAD_DEBOUNCER_

#define BOUNCE_WITH_PROMPT_DETECTION

#include <Bounce2.h>
#include "GamepadState.h"

class GamepadDebouncer : public Debouncer {
	public:
		GamepadDebouncer();
		GamepadDebouncer(uint16_t m, uint16_t i = 0, bool d = false)
			: inputMask(m), interval_millis(i), isDpad(d) { };

		bool isDpad;
		uint16_t inputMask;

		__attribute__((always_inline)) inline void setGamepadState(GamepadState &gamepadState) {
			this->gamepadState = &gamepadState;
		}

	protected:
		uint16_t interval_millis;
		GamepadState *gamepadState;

		virtual bool readCurrentState();
};

#endif
