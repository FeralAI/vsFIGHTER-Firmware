#include "VsFighterStorage.h"

#define STORAGE_DISABLE_STATUS_LED_INDEX (STORAGE_FIRST_AVAILBLE_INDEX) // 1 byte

void VsFighterStorage::get(int index, void *data, uint16_t size)
{
	uint8_t buffer[size] = { };
	for (int i = 0; i < size; i++)
		EEPROM.get(index + i, buffer[i]);

	memcpy(data, buffer, size);
}

void VsFighterStorage::set(int index, void *data, uint16_t size)
{
	uint8_t buffer[size] = { };
	memcpy(buffer, data, size);
	for (int i = 0; i < size; i++)
		EEPROM.put(index + i, buffer[i]);
}

bool VsFighterStorage::getDisableStatusLED()
{
	bool enabled = true;
	get(STORAGE_DISABLE_STATUS_LED_INDEX, &enabled, sizeof(bool));
	return enabled;
}

void VsFighterStorage::setDisabledStatusLED(bool enabled)
{
	set(STORAGE_DISABLE_STATUS_LED_INDEX, &enabled, sizeof(bool));
}
