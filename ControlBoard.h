/************************************************************************************
 * Control Board
 * 
 * Base class for implementing a gamepad/controller.
 ************************************************************************************/

#ifndef _CONTROL_BOARD_
#define _CONTROL_BOARD_

#include "GamepadState.h"

class ControlBoard {
	public:
		GamepadState state;

		/**
		 * Performs pin setup
		 */
		virtual void setupPins();

		/**
		 * Updates the gamepad state from the current inputs
		 */
		virtual void update(DpadMode dpadMode);

		HotkeyAction checkHotkeys();
		bool compareState(GamepadState otherState);
		uint16_t dpadToAnalogX();
		uint16_t dpadToAnalogY();

		bool isDpadUpPressed();
		bool isDpadDownPressed();
		bool isDpadLeftPressed();
		bool isDpadRightPressed();
		bool isFunctionPressed();
		bool isSelectPressed();
		bool isStartPressed();

		/**
		 * Retrieves the cleaned dpad state
		 */
		uint8_t runSOCD(bool up, bool down, bool left, bool right);
};

#endif
