#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "asprintf.h"

/* Implementation of asprintf() */

int
asprintf(char **restrict strp, const char *restrict fmt, ...)
{
	va_list args;
	int size = 0;
	va_start(args, fmt);
	va_end(args);
	return size = vasprintf(strp, fmt, args);
}

int
vasprintf(char **restrict strp, const char *restrict fmt, va_list ap)
{
	va_list args;
	va_copy(args, ap);
	int size = vsnprintf(NULL, 0, fmt, args);

	/* if negative number is returned return error */
	if(size < 0)
		return -1;
	*strp = (char *)malloc(size + 1);
	if(*strp == NULL)
		return -1;

	va_end(args);
	return size = vsprintf(*strp, fmt, ap);
}
