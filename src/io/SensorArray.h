#ifndef LINEFOLLOWERBOT_SENSORARRAY_H
#define LINEFOLLOWERBOT_SENSORARRAY_H
#include "pico/stdlib.h"

struct SensorReadings {
	float ir1, ir2, ir3, ir4, ir5;
};

class SensorArray {
private:
	uint i2c_sda_pin;
	uint i2c_scl_pin;

	uint8_t ads_address_1 = 0x48; // ADDR: GND
	uint8_t ads_address_2 = 0x49; // ADDR: 3.3V VDD

	uint16_t read_ads1115_channel(uint8_t address, uint8_t channel) const;

public:
	SensorArray(uint i2c_sda_pin, uint i2c_scl_pin);
	void init() const;
	SensorReadings read_all() const;
};


#endif