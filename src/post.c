#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>

#include "util.h"
#include "asprintf.h"

/* This function post "status" with the visibility "visibility" */


int
post_status(const char *status, const char *scope)
{
	char instance[50];
	char client_id[50];
	char client_secret[50];
	char access_token[50];
	
	get_tokens_from_file(".demiurgerc",instance,client_id,client_secret,access_token);
	CURL *curl = curl_easy_init();
	if(curl == NULL) {
		fprintf(stderr,"Error creating libcurl thing\n");
		return -1;
	}
	
	char *api_url = "/api/v1/statuses";
	char *url = NULL;
	
	asprintf(&url,"%s%s",instance,api_url);
	puts(url);
	
	curl_easy_setopt(curl,CURLOPT_URL,url);

	char *header_fmt = "Authorization: Bearer %s";
	char *authorization_header = NULL;
	struct curl_slist *header_list = NULL;
	
	asprintf(&authorization_header,header_fmt,access_token);
	header_list = curl_slist_append(header_list, authorization_header);

	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list);

	char *params_fmt = "status=%s&visibility=%s";

	char *status_to_post = NULL;
	char *visibility_to_post = NULL;
	
	char *post_params = NULL;

	asprintf(&post_params,params_fmt,status,scope);
	
	curl_easy_setopt(curl,CURLOPT_POSTFIELDS,post_params);
	
	curl_easy_perform(curl);

	free(url);
	free(post_params);
	free(status_to_post);
	free(visibility_to_post);
	free(authorization_header);
	
	return 0;

}
