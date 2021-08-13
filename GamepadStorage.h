#ifndef _PERSISTENT_STORAGE_
#define _PERSISTENT_STORAGE_

#define STORAGE_INPUT_MODE_INDEX 0
#define STORAGE_DPAD_MODE_INDEX  1
#define STORAGE_SOCD_MODE_INDEX  2

#include <EEPROM.h>
#include "GamepadState.h"

class GamepadStorage {
	public:
		DpadMode getDpadMode() {
			DpadMode mode = DpadMode::DIGITAL;
			get(STORAGE_DPAD_MODE_INDEX, mode);
			return mode;
		}

		DpadMode setDpadMode(DpadMode mode) {
			return set(STORAGE_DPAD_MODE_INDEX, mode);
		}

		InputMode getInputMode() {
			InputMode mode = InputMode::XINPUT;
			get(STORAGE_INPUT_MODE_INDEX, mode);
			return mode;
		}

		InputMode setInputMode(InputMode mode) {
			return set(STORAGE_INPUT_MODE_INDEX, mode);
		}

		SOCDMode getSOCDMode() {
			SOCDMode mode = SOCDMode::HITBOX;
			get(STORAGE_SOCD_MODE_INDEX, mode);
			return mode;
		}

		SOCDMode setSOCDMode(SOCDMode mode) {
			return set(STORAGE_SOCD_MODE_INDEX, mode);
		}
	
	protected:
		/**
		 * Wrapper for "get" call to storage API
		 */
		template <typename T> T GamepadStorage::get(int index, T &value) {
			return EEPROM.get(index, value);
		}

		/**
		 * Wrapper for "set" call to storage API
		 */
		template <typename T> T GamepadStorage::set(int index, const T &value) {
			return EEPROM.put(index, value);
		}
};

static GamepadStorage Storage;

#endif
