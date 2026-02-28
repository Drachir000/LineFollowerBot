#include "PowerLED.h"

#include "Log.h"
#include "pico/stdlib.h"

PowerLED::PowerLED(const uint red_pin, const uint green_pin, const uint blue_pin) {
	pin_r = red_pin;
	pin_g = green_pin;
	pin_b = blue_pin;
	current_color = OFF;
}

void PowerLED::init() const {
	Log::info("Initializing PowerLED with Pin_Red = %u, Pin_Green = %u, Pin_Blue = %u...", pin_r, pin_g, pin_b);

	gpio_init(pin_r);
	gpio_set_dir(pin_r, GPIO_OUT);
	gpio_init(pin_g);
	gpio_set_dir(pin_g, GPIO_OUT);
	gpio_init(pin_b);
	gpio_set_dir(pin_b, GPIO_OUT);
}

void PowerLED::set_color(const PowerLedColor color) {
	current_color = color;

	const bool r_state = (color == RED || color == YELLOW || color == VIOLET);
	const bool g_state = (color == GREEN || color == YELLOW || color == AQUA);
	const bool b_state = (color == BLUE || color == VIOLET || color == AQUA);

	gpio_put(pin_r, r_state);
	gpio_put(pin_g, g_state);
	gpio_put(pin_b, b_state);
}

PowerLedColor PowerLED::get_color() const {
	return current_color;
}

const char *led_color_name(const PowerLedColor color) {
	switch (color) {
		case RED: return "red";
		case GREEN: return "green";
		case BLUE: return "blue";
		case YELLOW: return "yellow";
		case VIOLET: return "violet";
		case AQUA: return "aqua";
		case OFF: return "off";
		default: return "unknown";
	}
}