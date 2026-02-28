#ifndef LINEFOLLOWERBOT_POWERLED_H
#define LINEFOLLOWERBOT_POWERLED_H

#include "pico/stdlib.h"

enum PowerLedColor { RED, GREEN, BLUE, YELLOW, AQUA, VIOLET, OFF };

class PowerLED {
private:
	uint pin_r;
	uint pin_g;
	uint pin_b;
	PowerLedColor current_color;

public:
	PowerLED(uint red_pin, uint green_pin, uint blue_pin);

	void init() const;

	void set_color(PowerLedColor color);

	[[nodiscard]] PowerLedColor get_color() const;
};

const char *led_color_name(PowerLedColor color);

#endif
