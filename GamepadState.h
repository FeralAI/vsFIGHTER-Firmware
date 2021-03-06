#ifndef _GAMEPAD_STATE_
#define _GAMEPAD_STATE_

#include <stdint.h>

#define GAMEPAD_ACTIVE_BUTTON_COUNT 14
#define GAMEPAD_DEBOUNCE_BUTTON_COUNT 18

#define GAMEPAD_DPAD_UP    (1U << 0)
#define GAMEPAD_DPAD_DOWN  (1U << 1)
#define GAMEPAD_DPAD_LEFT  (1U << 2)
#define GAMEPAD_DPAD_RIGHT (1U << 3)

#define GAMEPAD_BUTTON_01  (1U <<  0) // Generic: K1, Switch: B, Xbox: A
#define GAMEPAD_BUTTON_02  (1U <<  1) // Generic: K2, Switch: A, Xbox: B
#define GAMEPAD_BUTTON_03  (1U <<  2) // Generic: P1, Switch: Y, Xbox: X
#define GAMEPAD_BUTTON_04  (1U <<  3) // Generic: P2, Switch: X, Xbox: Y
#define GAMEPAD_BUTTON_05  (1U <<  4) // Generic: P4, Switch: L, Xbox: LB
#define GAMEPAD_BUTTON_06  (1U <<  5) // Generic: P3, Switch: R, Xbox: RB
#define GAMEPAD_BUTTON_07  (1U <<  6) // Generic: K4, Switch: ZL, Xbox: LT (Digital)
#define GAMEPAD_BUTTON_08  (1U <<  7) // Generic: K3, Switch: ZR, Xbox: RT (Digital)
#define GAMEPAD_BUTTON_09  (1U <<  8) // Generic: Select, Switch: -, Xbox: View
#define GAMEPAD_BUTTON_10  (1U <<  9) // Generic: Start, Switch: +, Xbox: Menu
#define GAMEPAD_BUTTON_11  (1U << 10) // All: Left Stick Click
#define GAMEPAD_BUTTON_12  (1U << 11) // All: Right Stick Click
#define GAMEPAD_BUTTON_13  (1U << 12) // All: Home/Logo
#define GAMEPAD_BUTTON_14  (1U << 13) // Switch: Capture
#define GAMEPAD_BUTTON_15  (1U << 14) // Unused
#define GAMEPAD_BUTTON_16  (1U << 15) // Unused

#define GAMEPAD_AXIS_MIN 0
#define GAMEPAD_AXIS_MID 0x7FFF
#define GAMEPAD_AXIS_MAX 0xFFFF

typedef enum {
	DIGITAL,
	LEFT_ANALOG,
	RIGHT_ANALOG,
} DpadMode;

typedef enum {
	UP_PRIORITY, // U+D=U, L+R=N
	NEUTRAL,     // U+D=N, L+R=N
	LAST_INPUT,  // U>D=D, L>R=R (Last Input Priority, aka Last Win)
} SOCDMode;

typedef enum {
	DIRECTION_NONE,
	DIRECTION_UP,
	DIRECTION_DOWN,
	DIRECTION_LEFT,
	DIRECTION_RIGHT
} Direction;

typedef enum {
	NONE              = 0x00,
	DPAD_DIGITAL      = 0x01,
	DPAD_LEFT_ANALOG  = 0x02,
	DPAD_RIGHT_ANALOG = 0x04,
	HOME_BUTTON       = 0x08,
	CAPTURE_BUTTON    = 0x10,
	SOCD_UP_PRIORITY  = 0x20,
	SOCD_NEUTRAL      = 0x40,
	SOCD_LAST_INPUT   = 0x80,
} HotkeyAction;

typedef struct {
	// Raw input values
	uint8_t dpadInputs : 4; // Raw D-Pad input states
	uint16_t buttonInputs;  // Raw button input states
	// Current values
	uint8_t dpad : 4;       // D-Pad/HAT switch (2 bytes) report data
	uint16_t buttons;       // Supports up to 16 digital buttons
	uint16_t lx;            // Left analog X-axis
	uint16_t ly;            // Left analog Y-axis
	uint16_t rx;            // Right analog X-axis
	uint16_t ry;            // Right analog Y-axis
	uint8_t lt;             // Left analog trigger
	uint8_t rt;             // Right analog trigger
} GamepadState;

/**
 * Check two GamepadStates for equality
 */
__attribute__((always_inline)) inline static bool areStatesEqual(GamepadState *a, GamepadState *b) {
	return a->dpad == b->dpad
	    && a->buttons == b->buttons
	    && a->lt == b->lt
	    && a->rt == b->rt
	    && a->lx == b->lx
	    && a->ly == b->ly
	    && a->rx == b->rx
	    && a->ry == b->ry;
}

/**
 * Convert the horizontal GamepadState dpad axis value into an analog value
 */
__attribute__((always_inline)) inline uint16_t dpadToAnalogX(uint8_t dpad) {
	switch (dpad & (GAMEPAD_DPAD_LEFT | GAMEPAD_DPAD_RIGHT)) {
		case GAMEPAD_DPAD_LEFT:
			return GAMEPAD_AXIS_MIN;
		case GAMEPAD_DPAD_RIGHT:
			return GAMEPAD_AXIS_MAX;
		default:
			return GAMEPAD_AXIS_MID;
	}
}

/**
 * Convert the vertical GamepadState dpad axis value into an analog value
 */
__attribute__((always_inline)) inline uint16_t dpadToAnalogY(uint8_t dpad) {
	switch (dpad & (GAMEPAD_DPAD_UP | GAMEPAD_DPAD_DOWN)) {
		case GAMEPAD_DPAD_UP:
			return GAMEPAD_AXIS_MIN;
		case GAMEPAD_DPAD_DOWN:
			return GAMEPAD_AXIS_MAX;
		default:
			return GAMEPAD_AXIS_MID;
	}
}

/**
 * Run the SOCD cleaner
 */
static uint8_t runSOCD(SOCDMode mode, uint8_t dpadValues) {
	static Direction lastUD = Direction::DIRECTION_NONE;
	static Direction lastLR = Direction::DIRECTION_NONE;
	uint8_t newValues = 0;

	switch (dpadValues & (GAMEPAD_DPAD_UP | GAMEPAD_DPAD_DOWN)) {
		case (GAMEPAD_DPAD_UP | GAMEPAD_DPAD_DOWN):
			if (mode == SOCDMode::UP_PRIORITY) {
				newValues |= GAMEPAD_DPAD_UP;
				lastUD = Direction::DIRECTION_UP;
			} else if (mode == SOCDMode::LAST_INPUT && lastUD != Direction::DIRECTION_NONE)
				newValues |= (lastUD == Direction::DIRECTION_UP) ? GAMEPAD_DPAD_DOWN : GAMEPAD_DPAD_UP;
			else
				lastUD = Direction::DIRECTION_NONE;
			break;
		case GAMEPAD_DPAD_UP:
			newValues |= GAMEPAD_DPAD_UP;
			lastUD = Direction::DIRECTION_UP;
			break;
		case GAMEPAD_DPAD_DOWN:
			newValues |= GAMEPAD_DPAD_DOWN;
			lastUD = Direction::DIRECTION_DOWN;
			break;
		default:
			lastUD = Direction::DIRECTION_NONE;
			break;
	}

	switch (dpadValues & (GAMEPAD_DPAD_LEFT | GAMEPAD_DPAD_RIGHT)) {
		case (GAMEPAD_DPAD_LEFT | GAMEPAD_DPAD_RIGHT):
			if (mode == SOCDMode::LAST_INPUT && lastLR != Direction::DIRECTION_NONE)
				newValues |= (lastLR == Direction::DIRECTION_LEFT) ? GAMEPAD_DPAD_RIGHT : GAMEPAD_DPAD_LEFT;
			else
				lastLR = Direction::DIRECTION_NONE;
			break;
		case GAMEPAD_DPAD_LEFT:
			newValues |= GAMEPAD_DPAD_LEFT;
			lastLR = Direction::DIRECTION_LEFT;
			break;
		case GAMEPAD_DPAD_RIGHT:
			newValues |= GAMEPAD_DPAD_RIGHT;
			lastLR = Direction::DIRECTION_RIGHT;
			break;
		default:
			lastLR = Direction::DIRECTION_NONE;
			break;
	}

	return newValues;
}

#endif