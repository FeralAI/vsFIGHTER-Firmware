#ifndef VSFIGHTER_STORAGE_H_
#define VSFIGHTER_STORAGE_H_

#include <string.h>
#include <GamepadStorage.h>
#include <EEPROM.h>

class VsFighterStorage : public GamepadStorage
{
	public:
		void get(int index, void *data, uint16_t size);
		void set(int index, void *data, uint16_t size);

		void start() { }
		void save() { }

		bool getDisableStatusLED();
		void setDisabledStatusLED(bool enabled);
};

#endif
