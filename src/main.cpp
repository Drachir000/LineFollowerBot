#include <cstdio>

#include "io/Button.h"
#include "io/Log.h"
#include "io/PowerLED.h"
#include "io/motor/DCGearMotor.h"
#include "pico/stdlib.h"

#define LED_RED_PIN 13
#define LED_GREEN_PIN 14
#define LED_BLUE_PIN 15
#define BUTTON_PIN 16
#define MOTOR_1_ENA_PIN 17
#define MOTOR_1_IN1_PIN 18
#define MOTOR_1_IN2_PIN 19

PowerLED power_led(LED_RED_PIN, LED_GREEN_PIN, LED_BLUE_PIN);
Button button(BUTTON_PIN);
DCGearMotor motor_1(MOTOR_1_ENA_PIN, MOTOR_1_IN1_PIN, MOTOR_1_IN2_PIN);

void init() {
	Log::init();

	Log::info("Initializing...");

	power_led.init();
	button.init();
	motor_1.init();

	Log::info("Initialized.");
}

enum ROBOT_STATE {
	IDLE,
	TASK_1,
	TASK_2,
	ERROR_STATE
};

PowerLedColor get_power_led_color(const ROBOT_STATE state) {
	switch (state) {
		case IDLE: return YELLOW;
		case TASK_1: return BLUE;
		case TASK_2: return GREEN;
		default: return RED;
	}
}

int main() {
	init();

	while (true) {
		power_led.setColor(YELLOW);

		button.wait_for_button_press();

		power_led.setColor(BLUE);
		motor_1.setSpeed(50.0f);

		sleep_ms(1000 * 5);

		power_led.setColor(YELLOW);
		motor_1.stop();

		sleep_ms(1000 * 10);

		power_led.setColor(RED);
		motor_1.setSpeed(-80.0f);

		sleep_ms(1000 * 5);

		power_led.setColor(YELLOW);
		motor_1.stop(true);
	}

	return 0;
}
