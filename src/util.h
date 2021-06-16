#include <stdlib.h>

int
get_tokens_from_file(char *filename, char *instance,char *client_id,
				 char *client_secret, char *access_token);
size_t
write_data(void *buffer, size_t size, size_t nmemb, void *userp);

void
eputs(const char *s);

struct memory {
	char *response;
	size_t size;
};

static size_t cb(void *data, size_t size, size_t nmemb, void *userp);
