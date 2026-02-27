#ifndef LINEFOLLOWERBOT_LOG_H
#define LINEFOLLOWERBOT_LOG_H
#include <stdio.h>

enum LogLevel { DEBUG, INFO, WARNING, ERROR };

class Log {
private:
	static bool initialized;

	static const char *level_color(LogLevel level);

	static const char *level_name(LogLevel level);

public:
	static void init();

	static void log(LogLevel level, const char *format, va_list args);

	static void debug(const char *format, ...);

	static void info(const char *format, ...);

	static void warning(const char *format, ...);

	static void error(const char *format, ...);
};


#endif
