#ifndef _VSFIGHTER_BOARD_
#define _VSFIGHTER_BOARD_

#include "ControlBoard.h"

// Pin mappings
#define PORT_PIN_UP     PF7 // A0
#define PORT_PIN_DOWN   PF6 // A1
#define PORT_PIN_LEFT   PF5 // A2
#define PORT_PIN_RIGHT  PF4 // A3
#define PORT_PIN_P1     PD2 // 1
#define PORT_PIN_P2     PD3 // 0
#define PORT_PIN_P3     PB1 // 15
#define PORT_PIN_P4     PD4 // 4
#define PORT_PIN_K1     PD0 // 3/SCL
#define PORT_PIN_K2     PD1 // 2/SDA
#define PORT_PIN_K3     PB6 // 10
#define PORT_PIN_K4     PD7 // 6
#define PORT_PIN_SELECT PB3 // 14
#define PORT_PIN_START  PB2 // 16
#define PORT_PIN_LS     PB4 // 8
#define PORT_PIN_RS     PB5 // 9

// Input masks
#define PORTB_INPUT_MASK 0b01111110
#define PORTD_INPUT_MASK 0b10011111
#define PORTF_INPUT_MASK 0b11110000

// Index of register state in read results
#define PORTB_INDEX 0
#define PORTD_INDEX 1
#define PORTF_INDEX 2

class VsFighterBoard: public ControlBoard {
	public:
		void setupPins() {
			// Set to input (invert mask to set to 0)
			DDRB = DDRB & ~PORTB_INPUT_MASK;
			DDRD = DDRD & ~PORTD_INPUT_MASK;
			DDRF = DDRF & ~PORTF_INPUT_MASK;

			// Set to high/pullup
			PORTB = PORTB | PORTB_INPUT_MASK;
			PORTD = PORTD | PORTD_INPUT_MASK;
			PORTF = PORTF | PORTF_INPUT_MASK;
		}

		void update(DpadMode dpadMode) {
			/* Run time currently 16-24μs, typically 20μs */

			uint8_t portStates[3];
			portStates[PORTB_INDEX] = ~PINB;
			portStates[PORTD_INDEX] = ~PIND;
			portStates[PORTF_INDEX] = ~PINF;

			state.hasAnalogTriggers = false;

			// Set D-Pad
			state.dpadInputs = 0
				| ((portStates[PORTF_INDEX] >> PORT_PIN_UP    & 1) ? 1 << 0 : 0)
				| ((portStates[PORTF_INDEX] >> PORT_PIN_DOWN  & 1) ? 1 << 1 : 0)
				| ((portStates[PORTF_INDEX] >> PORT_PIN_LEFT  & 1) ? 1 << 2 : 0)
				| ((portStates[PORTF_INDEX] >> PORT_PIN_RIGHT & 1) ? 1 << 3 : 0);

			state.dpad = runSOCD(
				state.dpadInputs & GAMEPAD_DPAD_UP,
				state.dpadInputs & GAMEPAD_DPAD_DOWN,
				state.dpadInputs & GAMEPAD_DPAD_LEFT,
				state.dpadInputs & GAMEPAD_DPAD_RIGHT
			);

			switch (dpadMode) {
				case DpadMode::LEFT_ANALOG:
					state.lx = dpadToAnalogX();
					state.ly = dpadToAnalogY();
					state.rx = GAMEPAD_AXIS_MID;
					state.ry = GAMEPAD_AXIS_MID;
					state.dpad = 0;
					break;
				case DpadMode::RIGHT_ANALOG:
					state.lx = GAMEPAD_AXIS_MID;
					state.ly = GAMEPAD_AXIS_MID;
					state.rx = dpadToAnalogX();
					state.ry = dpadToAnalogY();
					state.dpad = 0;
					break;
				default:
					state.lx = GAMEPAD_AXIS_MID;
					state.ly = GAMEPAD_AXIS_MID;
					state.rx = GAMEPAD_AXIS_MID;
					state.ry = GAMEPAD_AXIS_MID;
					break;
			}

			// Set buttons
			state.buttons = 0
				| ((portStates[PORTD_INDEX] >> PORT_PIN_K1 & 1)     ? 1 << 0  : 0) // Generic: K1, Switch: B, Xbox: A
				| ((portStates[PORTD_INDEX] >> PORT_PIN_K2 & 1)     ? 1 << 1  : 0) // Generic: K2, Switch: A, Xbox: B
				| ((portStates[PORTD_INDEX] >> PORT_PIN_P1 & 1)     ? 1 << 2  : 0) // Generic: P1, Switch: Y, Xbox: X
				| ((portStates[PORTD_INDEX] >> PORT_PIN_P2 & 1)     ? 1 << 3  : 0) // Generic: P2, Switch: X, Xbox: Y
				| ((portStates[PORTD_INDEX] >> PORT_PIN_P4 & 1)     ? 1 << 4  : 0) // Generic: P4, Switch: L, Xbox: LB
				| ((portStates[PORTB_INDEX] >> PORT_PIN_P3 & 1)     ? 1 << 5  : 0) // Generic: P3, Switch: R, Xbox: RB
				| ((portStates[PORTD_INDEX] >> PORT_PIN_K4 & 1)     ? 1 << 6  : 0) // Generic: K4, Switch: ZL, Xbox: LT (Digital)
				| ((portStates[PORTB_INDEX] >> PORT_PIN_K3 & 1)     ? 1 << 7  : 0) // Generic: K3, Switch: ZR, Xbox: RT (Digital)
				| ((portStates[PORTB_INDEX] >> PORT_PIN_SELECT & 1) ? 1 << 8  : 0) // Generic: Select, Switch: -, Xbox: View
				| ((portStates[PORTB_INDEX] >> PORT_PIN_START & 1)  ? 1 << 9  : 0) // Generic: Start, Switch: +, Xbox: Menu
				| ((portStates[PORTB_INDEX] >> PORT_PIN_LS & 1)     ? 1 << 10 : 0) // All: Left Stick Click
				| ((portStates[PORTB_INDEX] >> PORT_PIN_RS & 1)     ? 1 << 11 : 0) // All: Right Stick Click
			;

			// No analog support, center values
			state.lt = 0;
			state.rt = 0;
		}
};

#endif
