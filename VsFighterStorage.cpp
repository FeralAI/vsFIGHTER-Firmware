#include "VsFighterStorage.h"

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
	uint8_t disabled = 0;
	get(STORAGE_DISABLE_STATUS_LED_INDEX, &disabled, sizeof(uint8_t));
	return disabled;
}

void VsFighterStorage::setDisableStatusLED(bool disabled)
{
	uint8_t value = disabled ? 1 : 0;
	set(STORAGE_DISABLE_STATUS_LED_INDEX, &disabled, sizeof(uint8_t));
}
