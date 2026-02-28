//
// Created by mrab on 27.02.26.
//

#include "DCGearMotor.h"

#include "../Log.h"
#include "hardware/pwm.h"
#include "pico/stdlib.h"

#define MAX_PWM 65535

static bool isZeroSpeed(const float speed_percentage) {
	return speed_percentage > -0.1f && speed_percentage < 0.1f;
}

DCGearMotor::DCGearMotor(const uint pwm_pin, const uint dir1_pin, const uint dir2_pin) {
	this->pwm_pin = pwm_pin;
	this->dir1_pin = dir1_pin;
	this->dir2_pin = dir2_pin;
}

void DCGearMotor::init() {
	Log::info("Initializing DCGearMotor with PWM_Pin = %u, Dir1_Pin = %u, Dir2_Pin = %u...", pwm_pin, dir1_pin,
			dir2_pin);

	gpio_init(dir1_pin);
	gpio_set_dir(dir1_pin, GPIO_OUT);
	gpio_init(dir2_pin);
	gpio_set_dir(dir2_pin, GPIO_OUT);

	gpio_set_function(pwm_pin, GPIO_FUNC_PWM);
	pwm_slice = pwm_gpio_to_slice_num(pwm_pin);
	pwm_set_wrap(pwm_slice, MAX_PWM); // Max 16-bit
	pwm_set_enabled(pwm_slice, true);

	stop();
}

void DCGearMotor::setSpeed(float speed_percentage, const bool hard_brake) const {
	if (speed_percentage < -100) {
		speed_percentage = -100;
	} else if (speed_percentage > 100) {
		speed_percentage = 100;
	}

	gpio_put(dir1_pin, speed_percentage > 0 || (hard_brake && isZeroSpeed(speed_percentage)));
	gpio_put(dir2_pin, speed_percentage < 0 || (hard_brake && isZeroSpeed(speed_percentage)));

	if (isZeroSpeed(speed_percentage)) {
		pwm_set_gpio_level(pwm_pin, hard_brake ? MAX_PWM : 0);
	} else {
		if (speed_percentage < 0) {
			speed_percentage = -speed_percentage;
		}

		const uint16_t duty_cycle = (uint16_t) ((speed_percentage / 100.0f) * MAX_PWM);
		pwm_set_gpio_level(pwm_pin, duty_cycle);
	}
}

void DCGearMotor::stop(const bool hard_brake) const {
	setSpeed(0, hard_brake);
}
