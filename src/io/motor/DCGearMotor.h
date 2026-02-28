#ifndef LINEFOLLOWERBOT_DCGEARMOTOR_H
#define LINEFOLLOWERBOT_DCGEARMOTOR_H

#include "pico/stdlib.h"

class DCGearMotor {
private:
	uint pwm_pin;
	uint dir1_pin;
	uint dir2_pin;
	uint pwm_slice{};

public:
	DCGearMotor(uint pwm_pin, uint dir1_pin, uint dir2_pin);

	void init();

	void setSpeed(float speed_percentage, bool hard_brake = false) const;

	// Speed Percentage should be between -100% and 100%
	void stop(bool hard_brake = false) const;
};


#endif