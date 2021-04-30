#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>

#include "util.h"

/* This function post "status" with the visibility "visibility" */

/* TODO: Make the visibility thing work */

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
	char *url = (char*)malloc(strlen(instance) + strlen(api_url) + 1);
	
	if(url == NULL) {
		fprintf(stderr,"Error allocating memory!\n");
		return -1;
	}
	
	sprintf(url,"%s%s",instance,api_url);
	puts(url);
	
	curl_easy_setopt(curl,CURLOPT_URL,url);

	char *header_fmt = "Authorization: Bearer %s";
	int len = strlen(header_fmt) + strlen(access_token) + 1;
	char *authorization_header = (char*)malloc(len);
	if(authorization_header == NULL) {
		fprintf(stderr,"Error allocating memory!");
		return -1;
	}

	struct curl_slist *header_list = NULL;
	
	sprintf(authorization_header,header_fmt,access_token);
	header_list = curl_slist_append(header_list, authorization_header);

	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list);
	
	int status_len = strlen(status) +  strlen("status=") + 1;
	int scope_len = strlen(scope) + strlen("visibility=") + 1;

	char *status_fmt = "status=%s";
	char *scope_fmt  = "visibility=%s";

	char *status_to_post = (char*)malloc(status_len);
	char *visibility_to_post = (char*)malloc(scope_len);
		
	if(status_to_post == NULL) {
		fprintf(stderr,"Error allocating memory\n");
		return -1;
	}

	sprintf(status_to_post,status_fmt,status);
	sprintf(visibility_to_post,scope_fmt,scope);

	curl_easy_setopt(curl,CURLOPT_POSTFIELDSIZE,12L);
	curl_easy_setopt(curl,CURLOPT_POSTFIELDS,status_to_post);
	
	curl_easy_perform(curl);

	free(url);
	free(status_to_post);
	free(visibility_to_post);
	free(authorization_header);
	
	return 0;

}
