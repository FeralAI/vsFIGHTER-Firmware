#ifndef _DS3_REPORT_
#define _DS3_REPORT_

#include <stdint.h>

#define DS3_BUTTON_SELECT_MASK   0x01
#define DS3_BUTTON_L3_MASK       0x02
#define DS3_BUTTON_R3_MASK       0x04
#define DS3_BUTTON_START_MASK    0x08
#define DS3_DPAD_UP_MASK         0x10
#define DS3_DPAD_RIGHT_MASK      0x20
#define DS3_DPAD_DOWN_MASK       0x40
#define DS3_DPAD_LEFT_MASK       0x80
#define DS3_BUTTON_L2_MASK       0x100
#define DS3_BUTTON_R1_MASK       0x200
#define DS3_BUTTON_L1_MASK       0x400
#define DS3_BUTTON_R2_MASK       0x800
#define DS3_BUTTON_TRIANGLE_MASK 0x1000
#define DS3_BUTTON_CIRCLE_MASK   0x2000
#define DS3_BUTTON_CROSS_MASK    0x4000
#define DS3_BUTTON_SQUARE_MASK   0x8000
#define DS3_BUTTON_PS_MASK       0x10000

typedef struct {
	uint8_t reportId;
	byte reserved1;
	uint32_t buttons;
	uint8_t lx;
	uint8_t ly;
	uint8_t rx;
	uint8_t ry;
	byte reserved2[3];
	uint8_t analogDpadUp;
	uint8_t analogDpadRight;
	uint8_t analogDpadDown;
	uint8_t analogDpadLeft;
	uint8_t analogL2;
	uint8_t analogR2;
	uint8_t analogL1;
	uint8_t analogR1;
	uint8_t analogTriangle;
	uint8_t analogCircle;
	uint8_t analogCross;
	uint8_t analogSquare;
	byte reserved3[4];
	uint8_t batteryStatus;
	byte reserved4[10];
	uint16_t accelX;  // 10 bit
	uint16_t accellY; // 10 bit
	uint16_t accelZ;  // 10 bit
	uint16_t gyro;    // 10 bit
} DS3Report; // 48 bytes

// uint8_t dpadToHatDInput(uint8_t dpad) {
// 	switch (dpad & (DS3_DPAD_UP_MASK | DS3_DPAD_DOWN_MASK | DS3_DPAD_LEFT_MASK | DS3_DPAD_RIGHT_MASK)) {
// 		case DS3_DPAD_UP_MASK:                         return 0;
// 		case DS3_DPAD_UP_MASK | DS3_DPAD_RIGHT_MASK:   return 1;
// 		case DS3_DPAD_RIGHT_MASK:                      return 2;
// 		case DS3_DPAD_DOWN_MASK | DS3_DPAD_RIGHT_MASK: return 3;
// 		case DS3_DPAD_DOWN_MASK:                       return 4;
// 		case DS3_DPAD_DOWN_MASK | DS3_DPAD_LEFT_MASK:  return 5;
// 		case DS3_DPAD_LEFT_MASK:                       return 6;
// 		case DS3_DPAD_UP_MASK | DS3_DPAD_LEFT_MASK:    return 7;
// 	}

// 	return 15;
// }

// #ifdef PS3
// 	Gamepad._GamepadReport.hat = dpad2hat(axes);
// #else
// 	// UP + DOWN = UP, SOCD (Simultaneous Opposite Cardinal Directions) Cleaner
// 	if(axes & B10000000)
// 		Gamepad._GamepadReport.Y = -1;
// 	else if(axes & B01000000)
// 		Gamepad._GamepadReport.Y = 1;
// 	else
// 		Gamepad._GamepadReport.Y = 0;
// 	// UP + DOWN = NEUTRAL
// 	//Gamepad._GamepadReport.Y = ((axes & B01000000)>>6) - ((axes & B10000000)>>7);
// 	// LEFT + RIGHT = NEUTRAL
// 	Gamepad._GamepadReport.X = ((axes & B00010000)>>4) - ((axes & B00100000)>>5);
// #endif

// #ifdef PS3
// 	Gamepad._GamepadReport.buttons = buttons & 0x3FF;
// 	if(buttons & 0x400) // B11
// 		Gamepad._GamepadReport.Z = -1;
// 	else if(buttons & 0x800) // B12
// 		Gamepad._GamepadReport.Z = 1;
// 	else
// 		Gamepad._GamepadReport.Z = 0;
// #else
// 	Gamepad._GamepadReport.buttons = buttons;
// #endif

#endif