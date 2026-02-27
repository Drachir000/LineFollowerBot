#include "Log.h"

#include <cstdio>

#include <cstdarg>

#include "pico/stdlib.h"

#define COLOR_RESET   "\033[0m"
#define COLOR_CYAN    "\033[36m"  // For DEBUG
#define COLOR_GREEN   "\033[32m"  // For INFO
#define COLOR_YELLOW  "\033[33m"  // For WARNING
#define COLOR_RED     "\033[31m"  // For ERROR

bool Log::initialized = false;

void Log::init() {
	if (!initialized) {
		stdio_init_all(); // Init USB serial port -> for console
		sleep_ms(2000); // Give the USB serial connection time to establish

		initialized = true;
	}
}

const char *Log::level_color(const LogLevel level) {
	switch (level) {
		case DEBUG: return COLOR_CYAN;
		case INFO: return COLOR_GREEN;
		case WARNING: return COLOR_YELLOW;
		case ERROR: return COLOR_RED;
		default: return COLOR_RESET;
	}
}

const char *Log::level_name(const LogLevel level) {
	switch (level) {
		case DEBUG: return "DEBUG";
		case INFO: return "INFO";
		case WARNING: return "WARNING";
		case ERROR: return "ERROR";
		default: return "UNKNOWN";
	}
}

void Log::log(const LogLevel level, const char *format, const va_list args) {
	if (!initialized) {
		return;
	}

	const uint core = get_core_num();

	const uint32_t uptime = to_ms_since_boot(get_absolute_time());
	const uint32_t uptime_hours = uptime / (1000 * 60 * 60);
	const uint32_t uptime_minutes = (uptime / (1000 * 60)) % 60;
	const uint32_t uptime_seconds = (uptime / 1000) % 60;
	const uint32_t uptime_millis = uptime % 1000;

	// Prefix only
	printf("%s[%u:%02u:%02u.%03u] [Core %u] [%s]%s ",
			level_color(level),
			uptime_hours,
			uptime_minutes,
			uptime_seconds,
			uptime_millis,
			core,
			level_name(level),
			COLOR_RESET
	);

	// Message only, formatted
	vprintf(format, args);

	// Line break
	printf("\n");
}

void Log::debug(const char *format, ...) {
	va_list args;
	va_start(args, format);
	log(DEBUG, format, args);
	va_end(args);
}

void Log::info(const char *format, ...) {
	va_list args;
	va_start(args, format);
	log(INFO, format, args);
	va_end(args);
}

void Log::warning(const char *format, ...) {
	va_list args;
	va_start(args, format);
	log(WARNING, format, args);
	va_end(args);
}

void Log::error(const char *format, ...) {
	va_list args;
	va_start(args, format);
	log(ERROR, format, args);
	va_end(args);
}
