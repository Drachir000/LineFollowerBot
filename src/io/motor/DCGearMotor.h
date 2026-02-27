#ifndef LINEFOLLOWERBOT_DCGEARMOTOR_H
#define LINEFOLLOWERBOT_DCGEARMOTOR_H

#include "pico/stdlib.h"

class DCGearMotor {
private:
	uint pin_ena;
	uint pin_in1;
	uint pin_in2;
	uint pwm_slice{};

public:
	DCGearMotor(uint ena_pin, uint in1_pin, uint in2_pin);

	void init();

	void setSpeed(float speed_percentage, bool hard_brake = false) const;

	// Speed Percentage should be between -100% and 100%
	void stop(bool hard_brake = false) const;
};


#endif
