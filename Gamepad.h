/************************************************************************************
 * Gamepad
 * 
 * Base class for implementing a gamepad/controller.
 ************************************************************************************/

#ifndef _GAMEPAD_
#define _GAMEPAD_

#include "USB_HID.h"
#include "GamepadState.h"
#include "SwitchReport.h"
#include "XInputReport.h"

#define EEPROM_INPUT_MODE_INDEX 0
#define EEPROM_DPAD_MODE_INDEX  1
#define EEPROM_LED_MODE_INDEX   2

class Gamepad {
	public:
		Gamepad(uint32_t debounceMicros = 0);

		GamepadState previousState;
		GamepadState state;
		DpadMode dpadMode;
		InputMode inputMode;
		uint8_t ledMode = 0;

		/**
		 * Performs pin setup and other initialization
		 */
		virtual void setup();

		/**
		 * Retrieves the current inputs and updates the gamepad state
		 */
		virtual void read();

		/**
		 * Run debouncing algorithm against the cached GamepadState
		 */
		void debounce();

		HotkeyAction checkHotkeys();
		inline bool checkStateChanged(GamepadState otherState) __attribute__((always_inline));
		uint16_t dpadToAnalogX();
		uint16_t dpadToAnalogY();

		SwitchInputReport getSwitchReport();
		XInputReport getXInputReport();

		bool isDpadUpPressed();
		bool isDpadDownPressed();
		bool isDpadLeftPressed();
		bool isDpadRightPressed();
		bool isFunctionPressed();
		bool isLeftStickButtonPressed();
		bool isRightStickButtonPressed();
		bool isSelectPressed();
		bool isStartPressed();

		/**
		 * Retrieves the cleaned dpad state
		 */
		uint8_t runSOCD(bool up, bool down, bool left, bool right);

		/**
		 * Send the latest GamepadState to the USB Host.
		 */
		void update();

	protected:
		/**
		 * The debounce window (in microseconds) in which a button state change will not be allowed
		 */
		uint32_t debounceMicros = 0;

		uint32_t dpadDebouceTimers[4];
		uint32_t buttonDebounceTimers[GAMEPAD_ACTIVE_BUTTON_COUNT];
};

#endif
