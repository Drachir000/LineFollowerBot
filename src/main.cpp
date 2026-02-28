#include "io/Button.h"
#include "io/Log.h"
#include "io/PowerLED.h"
#include "io/motor/DCGearMotor.h"
#include "pico/mutex.h"
#include "pico/multicore.h"

#define LED_RED_PIN 13
#define LED_GREEN_PIN 14
#define LED_BLUE_PIN 15
#define BUTTON_PIN 16
#define MOTOR_LEFT_ENA_PIN 17
#define MOTOR_LEFT_IN1_PIN 18
#define MOTOR_LEFT_IN2_PIN 19

// Mutex Key
auto_init_mutex(robot_mutex);

struct RobotData {
	PowerLedColor power_led_color;

	float motor_left_speed;
	bool motor_left_hard_brake;

	//float motor_right_speed;
	//bool motor_right_hard_brake;
} shared_data;

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

void io_init(const PowerLED &power_led, DCGearMotor &motor_left) {
	Log::info("Initializing IO...");

	power_led.init();
	motor_left.init();
}

void io_loop() {
	PowerLED power_led(LED_RED_PIN, LED_GREEN_PIN, LED_BLUE_PIN);
	DCGearMotor motor_left(MOTOR_LEFT_ENA_PIN, MOTOR_LEFT_IN1_PIN, MOTOR_LEFT_IN2_PIN);

	io_init(power_led, motor_left);

	Log::info("Starting IO Loop...");
	while (true) {
		const absolute_time_t loop_start = get_absolute_time();

		mutex_enter_blocking(&robot_mutex);

		const PowerLedColor power_led_color = shared_data.power_led_color;
		const float motor_left_speed = shared_data.motor_left_speed;
		const bool motor_left_hard_brake = shared_data.motor_left_hard_brake;

		mutex_exit(&robot_mutex);

		power_led.set_color(power_led_color);
		motor_left.setSpeed(motor_left_speed, motor_left_hard_brake);

		// True 100Hz
		sleep_until(delayed_by_ms(loop_start, 10));
	}
}

void logic_loop(ROBOT_STATE robot_state = IDLE) {
	Log::info("Initializing Button...");
	const Button button(BUTTON_PIN);
	button.init();

	Log::info("Starting Logic Loop...");
	while (true) {
		const absolute_time_t loop_start = get_absolute_time();

		button.wait_for_button_press();

		mutex_enter_blocking(&robot_mutex);
		shared_data.power_led_color = VIOLET;
		shared_data.motor_left_speed = 50.0f;
		shared_data.motor_left_hard_brake = false;
		mutex_exit(&robot_mutex);

		button.wait_for_button_press(true, 200);

		mutex_enter_blocking(&robot_mutex);
		shared_data.power_led_color = RED;
		shared_data.motor_left_speed = -80.0f;
		shared_data.motor_left_hard_brake = false;
		mutex_exit(&robot_mutex);

		sleep_ms(2500);

		mutex_enter_blocking(&robot_mutex);
		shared_data.power_led_color = get_power_led_color(robot_state);
		shared_data.motor_left_speed = 0.0f;
		shared_data.motor_left_hard_brake = true;
		mutex_exit(&robot_mutex);

		// True 50Hz (concept for later, right now there is still the blocking wait_for_button_press call)
		sleep_until(delayed_by_ms(loop_start, 20));
	}
}

int main() {
	Log::init();

	Log::info("Initializing RobotData...");
	shared_data.power_led_color = get_power_led_color(IDLE);
	shared_data.motor_left_speed = 0.0f;
	shared_data.motor_left_hard_brake = false;

	// Core 1: I/O Loop
	multicore_launch_core1(io_loop);

	// Core 0: Logic Loop
	logic_loop();

	return 0;
}
