/************************************************************************************
 * Gamepad
 * 
 * Base class for implementing a gamepad/controller.
 * 
 * Depends on the Arduino EEPROM library.
 ************************************************************************************/

#ifndef _GAMEPAD_
#define _GAMEPAD_

#include "USB_HID.h"
#include "GamepadState.h"
#include "GamepadDebouncer.h"
#include "DS3Report.h"
#include "SwitchReport.h"
#include "XInputReport.h"
#include "GamepadStorage.h"

#ifndef DEBOUNCE_MILLIS
#define DEBOUNCE_MILLIS 0
#endif

class Gamepad {
	public:
		Gamepad() {
				#if DEBOUNCE_MILLIS > 0
					for (int i = 0; i < GAMEPAD_DEBOUNCE_BUTTON_COUNT; i++) {
						debouncers[i].setGamepadState(currentState);
					}
				#endif
		}

		GamepadState previousState;
		GamepadState currentState;

		DpadMode dpadMode        = DpadMode::DIGITAL;
		InputMode inputMode      = InputMode::XINPUT;
		SOCDMode socdMode        = SOCDMode::UP_PRIORITY;
		bool hasAnalogTriggers   = false;
		bool hasLeftAnalogStick  = false;
		bool hasRightAnalogStick = false;

		/**
		 * Load the saved configuration from persitent storage
		 */
		virtual void load();

		/**
		 * Perform pin setup and any other initialization the board requires
		 */
		virtual void setup();

		/**
		 * Retrieve the raw inputs and save to the current GamepadState
		 */
		virtual void read();

		/**
		 * Checks and executes any hotkey being pressed
		 */
		virtual HotkeyAction hotkey();

		/**
		 * Run debouncing algorithm against raw inputs in the current GamepadState
		 */
		void debounce();

		/**
		 * Process the raw inputs to fill the current GamepadState
		 */
		void process();

		/**
		 * Checks persistent storage and hotkeys to determine the input mode to use
		 */
		void configureInputMode();

		/**
		 * Send the latest GamepadState to the USB Host.
		 */
		void update();

		/**
		 * Returns if the function button/hotkey is pressed, override in derived board class
		 */
		virtual bool isDpadHotkeyPressed();

		/**
		 * Returns if the function button/hotkey is pressed, override in derived board class
		 */
		virtual bool isSOCDHotkeyPressed();

		DS3Report getDS3Report();
		SwitchInputReport getSwitchReport();
		XInputReport getXInputReport();

		__attribute__((always_inline)) inline bool isDpadUpPressed()           { return (currentState.dpadInputs   & GAMEPAD_DPAD_UP)    > 0; }
		__attribute__((always_inline)) inline bool isDpadDownPressed()         { return (currentState.dpadInputs   & GAMEPAD_DPAD_DOWN)  > 0; }
		__attribute__((always_inline)) inline bool isDpadLeftPressed()         { return (currentState.dpadInputs   & GAMEPAD_DPAD_LEFT)  > 0; }
		__attribute__((always_inline)) inline bool isDpadRightPressed()        { return (currentState.dpadInputs   & GAMEPAD_DPAD_RIGHT) > 0; }
		__attribute__((always_inline)) inline bool isLeftStickButtonPressed()  { return (currentState.buttonInputs & GAMEPAD_BUTTON_11)  > 0; }
		__attribute__((always_inline)) inline bool isRightStickButtonPressed() { return (currentState.buttonInputs & GAMEPAD_BUTTON_12)  > 0; }
		__attribute__((always_inline)) inline bool isSelectPressed()           { return (currentState.buttonInputs & GAMEPAD_BUTTON_09)  > 0; }
		__attribute__((always_inline)) inline bool isStartPressed()            { return (currentState.buttonInputs & GAMEPAD_BUTTON_10)  > 0; }

	protected:
#if DEBOUNCE_MILLIS > 0
		GamepadDebouncer debouncers[GAMEPAD_ACTIVE_BUTTON_COUNT + 4] = {
			GamepadDebouncer(GAMEPAD_DPAD_UP,    DEBOUNCE_MILLIS, true),
			GamepadDebouncer(GAMEPAD_DPAD_DOWN,  DEBOUNCE_MILLIS, true),
			GamepadDebouncer(GAMEPAD_DPAD_LEFT,  DEBOUNCE_MILLIS, true),
			GamepadDebouncer(GAMEPAD_DPAD_RIGHT, DEBOUNCE_MILLIS, true),
			GamepadDebouncer(GAMEPAD_BUTTON_01,  DEBOUNCE_MILLIS, false),
			GamepadDebouncer(GAMEPAD_BUTTON_02,  DEBOUNCE_MILLIS, false),
			GamepadDebouncer(GAMEPAD_BUTTON_03,  DEBOUNCE_MILLIS, false),
			GamepadDebouncer(GAMEPAD_BUTTON_04,  DEBOUNCE_MILLIS, false),
			GamepadDebouncer(GAMEPAD_BUTTON_05,  DEBOUNCE_MILLIS, false),
			GamepadDebouncer(GAMEPAD_BUTTON_06,  DEBOUNCE_MILLIS, false),
			GamepadDebouncer(GAMEPAD_BUTTON_07,  DEBOUNCE_MILLIS, false),
			GamepadDebouncer(GAMEPAD_BUTTON_08,  DEBOUNCE_MILLIS, false),
			GamepadDebouncer(GAMEPAD_BUTTON_09,  DEBOUNCE_MILLIS, false),
			GamepadDebouncer(GAMEPAD_BUTTON_10,  DEBOUNCE_MILLIS, false),
			GamepadDebouncer(GAMEPAD_BUTTON_11,  DEBOUNCE_MILLIS, false),
			GamepadDebouncer(GAMEPAD_BUTTON_12,  DEBOUNCE_MILLIS, false),
			GamepadDebouncer(GAMEPAD_BUTTON_13,  DEBOUNCE_MILLIS, false),
			GamepadDebouncer(GAMEPAD_BUTTON_14,  DEBOUNCE_MILLIS, false),
			// GamepadDebouncer(GAMEPAD_BUTTON_15, DEBOUNCE_MILLIS, false), // unused
			// GamepadDebouncer(GAMEPAD_BUTTON_16, DEBOUNCE_MILLIS, false), // unused
		}; // Buttons + dpad
#else
		GamepadDebouncer debouncers[0];
#endif
};

#endif
