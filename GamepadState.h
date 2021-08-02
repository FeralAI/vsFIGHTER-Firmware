#ifndef _GAMEPAD_STATE_
#define _GAMEPAD_STATE_

#include <stdint.h>

#define GAMEPAD_ACTIVE_BUTTON_COUNT 14

#define GAMEPAD_DPAD_UP    0x01
#define GAMEPAD_DPAD_DOWN  0x02
#define GAMEPAD_DPAD_LEFT  0x04
#define GAMEPAD_DPAD_RIGHT 0x08

#define GAMEPAD_BUTTON_01  0x01   // Generic: K1, Switch: B, Xbox: A
#define GAMEPAD_BUTTON_02  0x02   // Generic: K2, Switch: A, Xbox: B
#define GAMEPAD_BUTTON_03  0x04   // Generic: P1, Switch: Y, Xbox: X
#define GAMEPAD_BUTTON_04  0x08   // Generic: P2, Switch: X, Xbox: Y
#define GAMEPAD_BUTTON_05  0x10   // Generic: P4, Switch: L, Xbox: LB
#define GAMEPAD_BUTTON_06  0x20   // Generic: P3, Switch: R, Xbox: RB
#define GAMEPAD_BUTTON_07  0x40   // Generic: K4, Switch: ZL, Xbox: LT (Digital)
#define GAMEPAD_BUTTON_08  0x80   // Generic: K3, Switch: ZR, Xbox: RT (Digital)
#define GAMEPAD_BUTTON_09  0x100  // Generic: Select, Switch: -, Xbox: View
#define GAMEPAD_BUTTON_10  0x200  // Generic: Start, Switch: +, Xbox: Menu
#define GAMEPAD_BUTTON_11  0x400  // All: Left Stick Click
#define GAMEPAD_BUTTON_12  0x800  // All: Right Stick Click
#define GAMEPAD_BUTTON_13  0x1000 // All: Home/Logo
#define GAMEPAD_BUTTON_14  0x2000 // Switch: Capture
#define GAMEPAD_BUTTON_15  0x4000 // Unused
#define GAMEPAD_BUTTON_16  0x8000 // Unused

#define GAMEPAD_AXIS_MIN 0
#define GAMEPAD_AXIS_MID 0x7FFF
#define GAMEPAD_AXIS_MAX 0xFFFF

static const uint16_t PROGMEM GAMEPAD_DPAD_MASKS[] = {
	GAMEPAD_DPAD_UP,
	GAMEPAD_DPAD_DOWN,
	GAMEPAD_DPAD_LEFT,
	GAMEPAD_DPAD_RIGHT,
};

static const uint16_t PROGMEM GAMEPAD_BUTTON_MASKS[] = {
	GAMEPAD_BUTTON_01,
	GAMEPAD_BUTTON_02,
	GAMEPAD_BUTTON_03,
	GAMEPAD_BUTTON_04,
	GAMEPAD_BUTTON_05,
	GAMEPAD_BUTTON_06,
	GAMEPAD_BUTTON_07,
	GAMEPAD_BUTTON_08,
	GAMEPAD_BUTTON_09,
	GAMEPAD_BUTTON_10,
	GAMEPAD_BUTTON_11,
	GAMEPAD_BUTTON_12,
	GAMEPAD_BUTTON_13,
	GAMEPAD_BUTTON_14,
	GAMEPAD_BUTTON_15,
	GAMEPAD_BUTTON_16,
};

typedef enum {
	DIGITAL,
	LEFT_ANALOG,
	RIGHT_ANALOG,
} DpadMode;

typedef enum {
	NONE              = 0x00,
	DPAD_DIGITAL      = 0x01,
	DPAD_LEFT_ANALOG  = 0x02,
	DPAD_RIGHT_ANALOG = 0x04,
	HOME_BUTTON       = 0x08,
	CAPTURE_BUTTON    = 0x10,
} HotkeyAction;

typedef struct {
	// Raw input values
	uint8_t dpadInputs;     // Raw D-Pad input states
	uint16_t buttonInputs;  // Raw button input states
	// Current input values
	uint8_t dpad;           // D-Pad/HAT switch (2 bytes) report data
	uint16_t buttons;       // Supports up to 16 digital buttons
	uint16_t lx;            // Left analog X-axis
	uint16_t ly;            // Left analog Y-axis
	uint16_t rx;            // Right analog X-axis
	uint16_t ry;            // Right analog Y-axis
	uint8_t lt;             // Left analog trigger
	uint8_t rt;             // Right analog trigger
	bool hasAnalogTriggers; // Flag to indicate how to process trigger values
} GamepadState;

#endif