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
#include <curl/curl.h>
#include <json-c/json.h>
#include <string.h>
#include "util.h"
#include "asprintf.h"

char *
get_account_id(char *name)
{
	char instance[50];
	char client_id[50];
	char client_secret[50];
	char access_token[50];

	get_tokens_from_file(
		".demiurgerc", instance, client_id, client_secret, access_token);
	CURL *curl = curl_easy_init();
	if(curl == NULL) {
		fprintf(stderr, "Error creating libcurl thing\n");
		return NULL;
	}
	char *api_url_fmt = "%s/api/v1/accounts/%s/";
	char *api_url;
	dm_asprintf(&api_url, api_url_fmt, instance, name);
	struct json_object *parsed_json;
	struct json_object *account_id;

	/* For some reason I don't understand, you can't get users remote
	   users withotu authorization. So I have to pass the
	   authorization header. */

	char *header_fmt = "Authorization: Bearer %s";
	char *authorization_header = NULL;
	struct curl_slist *header_list = NULL;

	dm_asprintf(&authorization_header, header_fmt, access_token);
	header_list = curl_slist_append(header_list, authorization_header);

	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list);

	/* Curl options */
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list);
	curl_easy_setopt(curl, CURLOPT_URL, api_url);
	/* Create buffer for the result */

	struct memory chunk = { 0 };

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cb);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

	/* free */

	curl_easy_perform(curl);
	curl_easy_cleanup(curl);
	free(api_url);
	free(authorization_header);
	curl_slist_free_all(header_list);
	/* parse the thing */

	parsed_json = json_tokener_parse(chunk.response);
	json_object_object_get_ex(parsed_json, "id", &account_id);

	/* keep freeing */
	char *str = (char *)json_object_get_string(account_id);
	char *return_str = malloc(strlen(str + 1));
	strcpy(return_str, str); // strdup() segfaults for some reason
	json_object_put(parsed_json);
	free(chunk.response);
	return return_str;
}

int
follow_account(char *id, char action)
{
	char instance[50];
	char client_id[50];
	char client_secret[50];
	char access_token[50];
	get_tokens_from_file(
		".demiurgerc", instance, client_id, client_secret, access_token);
	CURL *curl = curl_easy_init();
	if(curl == NULL) {
		fprintf(stderr, "Error creating libcurl thing\n");
		return -1;
	}
	char *api_url_fmt = "%s/api/v1/accounts/%s/follow";
	if(action == 'f')
		api_url_fmt = "%s/api/v1/accounts/%s/follow";
	else if(action == 'u')
		api_url_fmt = "%s/api/v1/accounts/%s/unfollow";

	char *api_url;
	dm_asprintf(&api_url, api_url_fmt, instance, id);

	char *header_fmt = "Authorization: Bearer %s";
	char *authorization_header = NULL;
	struct curl_slist *header_list = NULL;

	dm_asprintf(&authorization_header, header_fmt, access_token);
	header_list = curl_slist_append(header_list, authorization_header);

	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list);
	curl_easy_setopt(curl, CURLOPT_POST, 1L);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "");
	curl_easy_setopt(curl, CURLOPT_URL, api_url);

	struct memory chunk = { 0 };

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cb);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

	curl_easy_perform(curl);
	
	curl_easy_cleanup(curl);
	free(api_url);
	free(authorization_header);
	curl_slist_free_all(header_list);

	struct json_object *parsed_json;
	struct json_object *following;

	parsed_json = json_tokener_parse(chunk.response);
	json_object_object_get_ex(parsed_json, "following", &following);
	printf("Following: %s\n", json_object_get_string(following));

	json_object_put(parsed_json);
	
	free(chunk.response);

	return 0;
}
