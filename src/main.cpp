#include <cstdio>

#include "io/Log.h"
#include "io/PowerLED.h"
#include "pico/stdlib.h"

#define LED_RED_PIN 13
#define LED_GREEN_PIN 14
#define LED_BLUE_PIN 15
#define BUTTON_PIN 16

PowerLED power_led(LED_RED_PIN, LED_GREEN_PIN, LED_BLUE_PIN);

void init() {
	Log::init();

	Log::info("Initializing...");

	power_led.init();

	gpio_init(BUTTON_PIN);
	gpio_set_dir(BUTTON_PIN, GPIO_IN);
	gpio_pull_up(BUTTON_PIN);

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

void wait_for_button_press(const uint button_pin, const bool wait_for_release, const bool debounce) {
	if (debounce) {
		sleep_ms(200);
	}

	while (wait_for_release && gpio_get(button_pin) == 0) {
		sleep_ms(10);
	}

	while (true) {
		if (gpio_get(button_pin) == 0) {
			break;
		}

		sleep_ms(10);
	}
}

int main() {
	init();

	while (true) {
		Log::info("Going into %s mode.", "IDLE");

		ROBOT_STATE state = IDLE;

		power_led.setColor(get_power_led_color(state));

		wait_for_button_press(BUTTON_PIN, true, true);
		Log::info("New state: %s.", "TASK_1");
		state = TASK_1;
		power_led.setColor(get_power_led_color(state));

		wait_for_button_press(BUTTON_PIN, true, true);
		Log::info("New state: %s.", "TASK_2");
		state = TASK_2;
		power_led.setColor(get_power_led_color(state));
		sleep_ms(1000 * 30);
	}

	return 0;
}
