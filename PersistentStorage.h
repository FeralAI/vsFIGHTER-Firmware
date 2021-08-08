#ifndef _PERSISTENT_STORAGE_
#define _PERSISTENT_STORAGE_

#define STORAGE_INPUT_MODE_INDEX 0
#define STORAGE_DPAD_MODE_INDEX  1
#define STORAGE_SOCD_MODE_INDEX  2

#include <EEPROM.h>

class PersistentStorage {
	public:
		template <typename T> T PersistentStorage::get(int index, T &value) {
			return EEPROM.get(index, value);
		}

		template <typename T> T PersistentStorage::put(int index, const T &value) {
			return EEPROM.put(index, value);
		}
};

static PersistentStorage Storage;

#endif
