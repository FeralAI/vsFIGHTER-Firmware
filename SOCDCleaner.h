#ifndef _SOCD_CLEANER_
#define _SOCD_CLEANER_

typedef enum {
	HITBOX, // U+D=U, L+R=N
	NEUTRAL // U+D=N, L+R=N
} SOCDMode;

/**
 * Run the SOCD cleaner
 */
__attribute__((always_inline)) inline uint8_t runSOCD(SOCDMode mode, bool up, bool down, bool left, bool right) {
	switch (mode) {
		case HITBOX:
			if (up && down)    { down = false; }
			if (left && right) { left = false; right = false; }
			break;
		case NEUTRAL:
			if (up && down)    { up = false; down = false; }
			if (left && right) { left = false; right = false; }
			break;
	}

	return 0
		^ (up ? GAMEPAD_DPAD_UP : 0)
		^ (down ? GAMEPAD_DPAD_DOWN : 0)
		^ (left ? GAMEPAD_DPAD_LEFT : 0)
		^ (right ? GAMEPAD_DPAD_RIGHT : 0)
	;
}

#endif