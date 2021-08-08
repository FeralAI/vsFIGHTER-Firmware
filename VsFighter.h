#ifndef _VSFIGHTER_
#define _VSFIGHTER_

// Define debounce time prior to including Gamepad.h, 0 to disable
#ifndef DEBOUNCE_MILLIS
#define DEBOUNCE_MILLIS 5
#endif
#include "Gamepad.h"

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

class VsFighter: public Gamepad {
	public:
		VsFighter() : Gamepad() {
			hasAnalogTriggers = false;
			hasLeftAnalogStick = false;
			hasRightAnalogStick = false;
		}

		/**
		 * Perform pin setup and any other initialization the board requires
		 */
		void setup() {
			// Set to input (invert mask to set to 0)
			DDRB = DDRB & ~PORTB_INPUT_MASK;
			DDRD = DDRD & ~PORTD_INPUT_MASK;
			DDRF = DDRF & ~PORTF_INPUT_MASK;

			// Set to high/pullup
			PORTB = PORTB | PORTB_INPUT_MASK;
			PORTD = PORTD | PORTD_INPUT_MASK;
			PORTF = PORTF | PORTF_INPUT_MASK;
		}

		/**
		 * Retrieve the raw inputs and save to the current GamepadState
		 */
		void read() {
			// Cache port states
			uint8_t ports[] = { ~PINB, ~PIND, ~PINF };

			// No analog triggers
			currentState.lt = 0;
			currentState.rt = 0;

			// Gather raw inputs
			currentState.dpadInputs = 0
				| ((ports[PORTF_INDEX] >> PORT_PIN_UP    & 1) ? GAMEPAD_DPAD_UP    : 0)
				| ((ports[PORTF_INDEX] >> PORT_PIN_DOWN  & 1) ? GAMEPAD_DPAD_DOWN  : 0)
				| ((ports[PORTF_INDEX] >> PORT_PIN_LEFT  & 1) ? GAMEPAD_DPAD_LEFT  : 0)
				| ((ports[PORTF_INDEX] >> PORT_PIN_RIGHT & 1) ? GAMEPAD_DPAD_RIGHT : 0);

			currentState.buttonInputs = 0
				| ((ports[PORTD_INDEX] >> PORT_PIN_K1 & 1)     ? GAMEPAD_BUTTON_01 : 0) // Generic: K1, Switch: B, Xbox: A
				| ((ports[PORTD_INDEX] >> PORT_PIN_K2 & 1)     ? GAMEPAD_BUTTON_02 : 0) // Generic: K2, Switch: A, Xbox: B
				| ((ports[PORTD_INDEX] >> PORT_PIN_P1 & 1)     ? GAMEPAD_BUTTON_03 : 0) // Generic: P1, Switch: Y, Xbox: X
				| ((ports[PORTD_INDEX] >> PORT_PIN_P2 & 1)     ? GAMEPAD_BUTTON_04 : 0) // Generic: P2, Switch: X, Xbox: Y
				| ((ports[PORTD_INDEX] >> PORT_PIN_P4 & 1)     ? GAMEPAD_BUTTON_05 : 0) // Generic: P4, Switch: L, Xbox: LB
				| ((ports[PORTB_INDEX] >> PORT_PIN_P3 & 1)     ? GAMEPAD_BUTTON_06 : 0) // Generic: P3, Switch: R, Xbox: RB
				| ((ports[PORTD_INDEX] >> PORT_PIN_K4 & 1)     ? GAMEPAD_BUTTON_07 : 0) // Generic: K4, Switch: ZL, Xbox: LT (Digital)
				| ((ports[PORTB_INDEX] >> PORT_PIN_K3 & 1)     ? GAMEPAD_BUTTON_08 : 0) // Generic: K3, Switch: ZR, Xbox: RT (Digital)
				| ((ports[PORTB_INDEX] >> PORT_PIN_SELECT & 1) ? GAMEPAD_BUTTON_09 : 0) // Generic: Select, Switch: -, Xbox: View
				| ((ports[PORTB_INDEX] >> PORT_PIN_START & 1)  ? GAMEPAD_BUTTON_10 : 0) // Generic: Start, Switch: +, Xbox: Menu
				| ((ports[PORTB_INDEX] >> PORT_PIN_LS & 1)     ? GAMEPAD_BUTTON_11 : 0) // All: Left Stick Click
				| ((ports[PORTB_INDEX] >> PORT_PIN_RS & 1)     ? GAMEPAD_BUTTON_12 : 0) // All: Right Stick Click
			;
		}
};

#endif
