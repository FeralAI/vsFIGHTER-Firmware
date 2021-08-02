#ifndef _SWITCH_REPORT_
#define _SWITCH_REPORT_

#include <stdint.h>

// HAT report (4 bits)
#define SWITCH_DPAD_VALUE_UP        0x00
#define SWITCH_DPAD_VALUE_UPRIGHT   0x01
#define SWITCH_DPAD_VALUE_RIGHT     0x02
#define SWITCH_DPAD_VALUE_DOWNRIGHT 0x03
#define SWITCH_DPAD_VALUE_DOWN      0x04
#define SWITCH_DPAD_VALUE_DOWNLEFT  0x05
#define SWITCH_DPAD_VALUE_LEFT      0x06
#define SWITCH_DPAD_VALUE_UPLEFT    0x07
#define SWITCH_DPAD_VALUE_NOTHING   0x08

// Button report (16 bits)
#define SWITCH_MASK_Y               0x01
#define SWITCH_MASK_B               0x02
#define SWITCH_MASK_A               0x04
#define SWITCH_MASK_X               0x08
#define SWITCH_MASK_L               0x10
#define SWITCH_MASK_R               0x20
#define SWITCH_MASK_ZL              0x40
#define SWITCH_MASK_ZR              0x80
#define SWITCH_MASK_MINUS           0x100
#define SWITCH_MASK_PLUS            0x200
#define SWITCH_MASK_L3              0x400
#define SWITCH_MASK_R3              0x800
#define SWITCH_MASK_HOME            0x1000
#define SWITCH_MASK_CAPTURE         0x2000

// Switch analog sticks only report 8 bits
#define SWITCH_ANALOG_MIN 0
#define SWITCH_ANALOG_MID 128
#define SWITCH_ANALOG_MAX 255

typedef struct {
	uint16_t Button; // 16 buttons; see JoystickButtons_t for bit mapping
	uint8_t  HAT;    // HAT switch; one nibble w/ unused nibble
	uint8_t  LX;     // Left  Stick X
	uint8_t  LY;     // Left  Stick Y
	uint8_t  RX;     // Right Stick X
	uint8_t  RY;     // Right Stick Y
	uint8_t  VendorSpec;
} SwitchInputReport;

typedef struct {
	uint16_t Button; // 16 buttons; see JoystickButtons_t for bit mapping
	uint8_t  HAT;    // HAT switch; one nibble w/ unused nibble
	uint8_t  LX;     // Left  Stick X
	uint8_t  LY;     // Left  Stick Y
	uint8_t  RX;     // Right Stick X
	uint8_t  RY;     // Right Stick Y
} SwitchOutputReport;

__attribute__((always_inline)) static inline bool checkSwitchReportChanged(SwitchInputReport *lastReport, SwitchInputReport *nextReport) {
	return &lastReport->Button != &nextReport->Button
	    || &lastReport->HAT != &nextReport->HAT
	    || &lastReport->LX != &nextReport->LX
	    || &lastReport->LY != &nextReport->LY
	    || &lastReport->RX != &nextReport->RX
	    || &lastReport->RY != &nextReport->RY;
}

#endif