#ifndef UTIL_H
#define UTIL_H




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

#include <stdlib.h>
#include <string.h>

struct config
{
	char instance[64];
	char client_id[64];
	char client_secret[64];
	char access_token[64];
};

int
load_config(struct config *config);

int
store_config(const struct config *config);

int
config_exists(void);

size_t
write_data(void *buffer, size_t size, size_t nmemb, void *userp);

void
eputs(const char *s);

static inline char *
dm_strncpy(char *dest, const char *src, size_t size)
{
	/* I wish strlcpy was standard C function */
	char *s = strncpy(dest, src, size);
	dest[size - 1] = 0;
	return s;
}

struct memory
{
	char *response;
	size_t size;
};

static inline void
free_response(struct memory *memory)
{
	if(!memory)
		return;
	free(memory->response);
	memory->response = 0;
	memory->size = 0;
}

size_t
cb(void *data, size_t size, size_t nmemb, void *userp);
#endif /* UTIL_H */
