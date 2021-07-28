/************************************************************************************
 * Control Board
 * 
 * Base class for implementing a gamepad/controller.
 ************************************************************************************/

#ifndef _CONTROL_BOARD_
#define _CONTROL_BOARD_

#include "UsbGamepad.h"

typedef enum {
	DIGITAL,
	LEFT_ANALOG,
	RIGHT_ANALOG,
} DpadMode;

class ControlBoard {
	public:
		GamepadState state;

		/**
		 * Performs pin setup
		 */
		virtual void setupPins();

		/**
		 * Retrieves the input states from all ports
		 */
		virtual void update(DpadMode dpadMode);

		void clearHotkeyState();
		bool compareState(GamepadState otherState);
		uint16_t dpadToAnalogX();
		uint16_t dpadToAnalogY();

		bool isDpadUpPressed();
		bool isDpadDownPressed();
		bool isDpadLeftPressed();
		bool isDpadRightPressed();
		bool isHotkeyPressed();
		bool isSelectPressed();
		bool isStartPressed();

		void pressHomeButton();
};

#endif
