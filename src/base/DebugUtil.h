#ifndef DEBUGUTIL_H_
#define DEBUGUTIL_H_

#include <iostream>
#include <cstdarg>
#include <cstdio>

/**
 * @brief Sets current debug level of ReaSE
 *
 * The higher the level the more debug output is generated.
 * Maximum level: 2, minimum level: 0 (no debug output).
 */
#define DEBUG_LEVEL 2

#if DEBUG_LEVEL == 2

/// Call this method to generate detailed debugging output
void debug2(const char *fmt, ...)
{
	char buf[1024];
	int n;
	va_list args;
	va_start(args, fmt);
	n = vsprintf(buf, fmt, args);
	va_end(args);
	std::cerr<<buf<<std::endl;
}

/// Call this method to generate moderate debugging output
void debug1(const char *fmt, ...)
{
	char buf[1024];
	int n;
	va_list args;
	va_start(args, fmt);
	n = vsprintf(buf, fmt, args);
	va_end(args);
	std::cerr<<buf<<std::endl;
}

#elif DEBUG_LEVEL == 1

inline void debug2(const char *fmt, ...) {;}
void debug1(const char *fmt, ...)
{
	char buf[1024];
	int n;
	va_list args;
	va_start(args, fmt);
	n = vsprintf(buf, fmt, args);
	va_end(args);
	std::cerr<<buf<<std::endl;
}

#elif DEBUG_LEVEL == 0

inline void debug2(const char *fmt, ...) {;}
inline void debug1(const char *fmt, ...) {;}

#endif

#endif /*DEBUGUTIL_H_*/
