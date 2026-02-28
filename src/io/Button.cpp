#include "Button.h"

#include "Log.h"
#include "pico/stdlib.h"

Button::Button(const uint pin) {
	this->pin = pin;
}

void Button::init() const {
	Log::info("Initializing Button with Pin = %u...", pin);

	gpio_init(pin);
	gpio_set_dir(pin, GPIO_IN);
	gpio_pull_up(pin);
}

bool Button::is_pressed() const {
	return gpio_get(pin) == 0;
}

void Button::wait_for_button_press(const bool wait_for_release, const uint debounce) const {
	if (debounce > 0) {
		sleep_ms(debounce);
	}

	while (wait_for_release && is_pressed()) {
		sleep_ms(10);
	}

	while (true) {
		if (is_pressed()) {
			break;
		}

		sleep_ms(10);
	}
}