#include <stdarg.h>

int
asprintf(char **restrict strp, const char *restrict fmt,... );

int
vasprintf(char **restrict strp, const char *restrict fmt,
	va_list ap);
