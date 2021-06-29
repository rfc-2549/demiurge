/*
  This file is part of demiurge.

  demiurge is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  demiurge is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with demiurge.  If not, see <https://www.gnu.org/licenses/>.
*/

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
