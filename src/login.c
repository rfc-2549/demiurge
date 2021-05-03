#include "config.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <readline/readline.h>

#include <json-c/json.h>

size_t
write_data(void *buffer, size_t size, size_t nmemb, void *userp)
{

	memcpy(userp,buffer,nmemb*size);
	return 0;
}


void
store_config(const char *instance, const char *client_id,
	const char *client_secret, const char *access_token)
{
	FILE *fp = fopen(".demiurgerc","w+");
	
	fprintf(fp,"%s=%s\n","instance",instance);
	fprintf(fp,"%s=%s\n","client_id",client_id);
	fprintf(fp,"%s=%s\n","client_secret",client_secret);
	fprintf(fp,"%s=%s\n","access_token",access_token);
	fclose(fp);
}

/* I am confident to present you, without a doubt, the worst code i've
 * ever written */

int
setup()
{
	char *instance = NULL;
	instance = readline("Enter your instance (e.g. https://social.fnord.tld) ");
	
	char *api_url = "/api/v1/apps";
	
	CURL *curl = curl_easy_init();

	struct json_object *parsed_json;
	struct json_object *json_client_id;
	struct json_object *json_client_secret;
	struct json_object *json_access_token;
	
	if(curl == NULL) {
		fprintf(stderr,"Error creating curl (what?)\n");
		return -1;
	}
	
	int len = strlen(instance) + strlen(api_url) + 1;
	char buf[8192];

	char *post_url = (char*)malloc(len);
	if(post_url == NULL) {
		fprintf(stderr,"Error allocating memory\n");
		return -1;
	}
	sprintf(post_url,"%s%s",instance,api_url);
	curl_easy_setopt(curl,CURLOPT_URL, post_url);
	curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl,CURLOPT_WRITEDATA,buf);
	
	/* TODO: Better way lol */
	curl_easy_setopt(curl,CURLOPT_POSTFIELDS,
		"client_name=demiurge&redirect_uris=urn:ietf:wg:oauth:2.0:oob&scope=read write");
	
	curl_easy_perform(curl);
	curl_easy_cleanup(curl);
	parsed_json = json_tokener_parse(buf);
	if(parsed_json == NULL) {
		fprintf(stderr,"error\n");
		return -1;
	}
	
	json_object_object_get_ex(parsed_json, "client_id", &json_client_id);
	json_object_object_get_ex(parsed_json, "client_secret", &json_client_secret);
	const char *client_id = json_object_get_string(json_client_id);
	const char *client_secret = json_object_get_string(json_client_secret);
	
	char *fmt = "%s%sresonse_type=code&client_id=%s&redirect_uri=urn:ietf:wg:oauth:2.0:oob&force_login&scope=read write";
	api_url = "/oauth/authorize?";
	
	len = strlen(instance) + strlen(api_url)+  strlen(fmt) + strlen(client_id) + 1;
	free(post_url);

	post_url = (char*)malloc(len);
	if(post_url == NULL) {
		fprintf(stderr,"Error allocating memory\n");
		return -1;
	}

	sprintf(post_url,fmt,instance,api_url,client_id);
	puts(post_url);

	curl = curl_easy_init();
	
	char *code = NULL;
	code = readline("Please log in in that url and paste the code here: ");

	char *access_token_fmt = "client_id=%s&client_secret=%s&redirect_uri=urn:ietf:wg:oauth:2.0:oob&grant_type=authorization_code&code=%s&scope=read write";
	api_url = "/oauth/token";

	
	char *post_token_url = (char*)malloc(strlen(access_token_fmt) + strlen(client_id)
		+ strlen(client_secret ) + strlen(code) + 3);
	if(post_token_url == NULL) {
		fprintf(stderr,"Error allocating memory");
		return -1;
	}
	sprintf(post_token_url, access_token_fmt,client_id,client_secret,code);

	len = strlen(instance) + strlen(api_url) + 1;

	post_url = (char*)malloc(len);
	if(post_url == NULL) {
		fprintf(stderr,"Error allocating memory");
		return -1;
	}
	sprintf(post_url,"%s%s",instance,api_url);

	curl_easy_setopt(curl, CURLOPT_URL,post_url);
	curl_easy_setopt(curl,CURLOPT_POSTFIELDS, post_token_url);
	curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl,CURLOPT_WRITEDATA,buf);
	curl_easy_perform(curl);

	parsed_json = json_tokener_parse(buf);
	json_object_object_get_ex(parsed_json, "access_token", &json_access_token);
	const char *access_token = json_object_get_string(json_access_token);
	store_config(instance,client_id,client_secret,access_token);	
	return 0;
}
