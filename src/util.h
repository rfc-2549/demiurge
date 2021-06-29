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

int
get_tokens_from_file(char *filename,
				 char *instance,
				 char *client_id,
				 char *client_secret,
				 char *access_token);
size_t
write_data(void *buffer, size_t size, size_t nmemb, void *userp);

void
eputs(const char *s);

struct memory
{
	char *response;
	size_t size;
};

size_t
cb(void *data, size_t size, size_t nmemb, void *userp);
