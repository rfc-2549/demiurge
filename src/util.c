#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct memory {
	char *response;
	size_t size;
};


/* This function fucking sucks and should use something else. I'll
 * have to rewrite this shit function some day. But for now, it will
 * do the job.
 * TODO: Check for errors
 */

/* Be aware with buffer overflows, fscanf() */


int
get_tokens_from_file(char *filename, char *instance, char *client_id,
				 char *client_secret, char *access_token)
{
	FILE *fp = fopen(filename,"r");
	if(fp == NULL)
		return -1;
	fscanf(fp,"instance=%s\n",instance);
	fscanf(fp,"client_id=%s\n",client_id);
	fscanf(fp,"client_secret=%s\n",client_secret);
	fscanf(fp,"access_token=%s\n",access_token);
	
	return 0;
}
 
size_t cb(void *data, size_t size, size_t nmemb, void *userp)
{
	size_t realsize = size * nmemb;
	struct memory *mem = (struct memory *)userp;
 
	char *ptr = realloc(mem->response, mem->size + realsize + 1);
	if(ptr == NULL)
		return 0;  /* out of memory! */
 
	mem->response = ptr;
	memcpy(&(mem->response[mem->size]), data, realsize);
	mem->size += realsize;
	mem->response[mem->size] = 0;
 
	return realsize;
}
 

size_t
write_data(void *buffer, size_t size, size_t nmemb, void *userp)
{

	memcpy(userp,buffer,nmemb*size);
	return 0;
}

/* like puts() but writes to stderr */

void
eputs(const char *s)
{
	fprintf(stderr,"%s\n",s);
}
