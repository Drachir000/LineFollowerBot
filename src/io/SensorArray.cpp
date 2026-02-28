#include "SensorArray.h"

#include "Log.h"
#include "hardware/i2c.h"

#define CONVERSION_REG 0x00
#define CONFIG_REG 0x01
#define I2C_BAUDRATE (400 * 1000) // I2C at 400kHz
#define MAX_VALUE 26000.0f

SensorArray::SensorArray(const uint i2c_sda_pin, const uint i2c_scl_pin) {
	this->i2c_sda_pin = i2c_sda_pin;
	this->i2c_scl_pin = i2c_scl_pin;
}

void SensorArray::init() const {
	Log::info("Initializing I2C for sensor array with a baudrate of %dHz and SDA_Pin = %u, SCL_Pin = %u ...", I2C_BAUDRATE, i2c_sda_pin, i2c_scl_pin);

	i2c_init(i2c0, I2C_BAUDRATE);
	gpio_set_function(i2c_sda_pin, GPIO_FUNC_I2C);
	gpio_set_function(i2c_scl_pin, GPIO_FUNC_I2C);
	gpio_pull_up(i2c_sda_pin);
	gpio_pull_up(i2c_scl_pin);
}

uint16_t SensorArray::read_ads1115_channel(const uint8_t address, const uint8_t channel) const {
	if (channel > 3) return 0;

	uint16_t config;
	switch (channel) {
		case 1:
			config = 0xD3E3;
			break;
		case 2:
			config = 0xE3E3;
			break;
		case 3:
			config = 0xF3E3;
			break;
		default: // Channel 0
			config = 0xC3E3;
			break;
	}

	uint8_t config_bytes[3];
	config_bytes[0] = CONFIG_REG;
	config_bytes[1] = (config >> 8) & 0xFF;
	config_bytes[2] = config & 0xFF;

	// send config to specified address
	const int write_ret = i2c_write_blocking(i2c0, address, config_bytes, 3, false);
	if (write_ret < 0) {
		Log::error("I2C WRITE FAIL at address 0x%02X!", address);
		return 0;
	}

	sleep_ms(2);

	uint8_t reg = CONVERSION_REG;
	i2c_write_blocking(i2c0, address, &reg, 1, true);

	uint8_t data[2];
	i2c_read_blocking(i2c0, address, data, 2, false);

	const int16_t raw_val = (data[0] << 8) | data[1];
	return (raw_val < 0) ? 0 : (uint16_t)raw_val;
}

SensorReadings SensorArray::read_all() const {
	SensorReadings readings;

	// IR 1-4 from ads1115 1 (0x48)
	readings.ir1 = read_ads1115_channel(ads_address_1, 0) / MAX_VALUE;
	readings.ir2 = read_ads1115_channel(ads_address_1, 1) / MAX_VALUE;
	readings.ir3 = read_ads1115_channel(ads_address_1, 2) / MAX_VALUE;
	readings.ir4 = read_ads1115_channel(ads_address_1, 3) / MAX_VALUE;

	// IR 5 from ads1115 2 (0x49)
	readings.ir5 = read_ads1115_channel(ads_address_2, 0) / MAX_VALUE;

	if(readings.ir1 > 1.0f) readings.ir1 = 1.0f;
	if(readings.ir2 > 1.0f) readings.ir2 = 1.0f;
	if(readings.ir3 > 1.0f) readings.ir3 = 1.0f;
	if(readings.ir4 > 1.0f) readings.ir4 = 1.0f;
	if(readings.ir5 > 1.0f) readings.ir5 = 1.0f;

	return readings;
}