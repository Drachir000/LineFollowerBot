//
// Created by mrab on 27.02.26.
//

#include "DCGearMotor.h"

#include "hardware/pwm.h"
#include "pico/stdlib.h"

#define MAX_PWM 65535

DCGearMotor::DCGearMotor(uint ena_pin, uint in1_pin, uint in2_pin) {
	pin_ena = ena_pin;
	pin_in1 = in1_pin;
	pin_in2 = in2_pin;
}

void DCGearMotor::init() {
	gpio_init(pin_in1);
	gpio_set_dir(pin_in1, GPIO_OUT);
	gpio_init(pin_in2);
	gpio_set_dir(pin_in2, GPIO_OUT);

	gpio_set_function(pin_ena, GPIO_FUNC_PWM);
	pwm_slice = pwm_gpio_to_slice_num(pin_ena);
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

	gpio_put(pin_in1, speed_percentage > 0 || (hard_brake && (speed_percentage > -0.1f && speed_percentage < 0.1f)));
	gpio_put(pin_in2, speed_percentage < 0 || (hard_brake && (speed_percentage > -0.1f && speed_percentage < 0.1f)));

	if (speed_percentage == 0) {
		pwm_set_gpio_level(pin_ena, hard_brake ? MAX_PWM : 0);
	} else {
		if (speed_percentage < 0) {
			speed_percentage = -speed_percentage;
		}

		uint16_t duty_cycle = (uint16_t) ((speed_percentage / 100.0f) * MAX_PWM);
		pwm_set_gpio_level(pin_ena, duty_cycle);
	}
}

void DCGearMotor::stop(const bool hard_brake) const {
	setSpeed(0, hard_brake);
}
