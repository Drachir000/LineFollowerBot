#ifndef LINEFOLLOWERBOT_BUTTON_H
#define LINEFOLLOWERBOT_BUTTON_H
#include "pico/types.h"


class Button {
private:
	uint pin;

public:
	explicit Button(uint pin);

	void init() const;

	[[nodiscard]] bool is_pressed() const;

	void wait_for_button_press(bool wait_for_release = false, uint debounce = 0) const;
};


#endif
