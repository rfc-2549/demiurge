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

#include "config.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <readline/readline.h>
#include <json-c/json.h>
#include "asprintf.h"
#include "util.h"

void
store_config(const char *instance,
		   const char *client_id,
		   const char *client_secret,
		   const char *access_token)
{
	FILE *fp = fopen(".demiurgerc", "w+");

	fprintf(fp, "%s=%s\n", "instance", instance);
	fprintf(fp, "%s=%s\n", "client_id", client_id);
	fprintf(fp, "%s=%s\n", "client_secret", client_secret);
	fprintf(fp, "%s=%s\n", "access_token", access_token);
	fclose(fp);
}

/* I am confident to present you, without a doubt, the worst code i've
 * ever written */

int
setup()
{
	char *instance = NULL;
	instance =
		readline("Enter your instance (e.g. https://social.fnord.tld) ");

	char *api_url = "/api/v1/apps";

	CURL *curl = curl_easy_init();

	struct json_object *parsed_json;
	struct json_object *json_client_id;
	struct json_object *json_client_secret;
	struct json_object *json_access_token;

	if(curl == NULL) {
		fprintf(stderr, "Error creating curl (what?)\n");
		return -1;
	}

	char buf[8192];

	char *post_url = NULL;

	dm_asprintf(&post_url, "%s%s", instance, api_url);
	curl_easy_setopt(curl, CURLOPT_URL, post_url);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, buf);

	/* TODO: Better way lol */
	/* If someone wants to do it, use curl_mime thing, I am to lazy
	 * to do this */

	curl_easy_setopt(curl,
				  CURLOPT_POSTFIELDS,
				  "client_name=demiurge&redirect_uris=urn:ietf:wg:oauth:2."
				  "0:oob&scope=read write follow");

	curl_easy_perform(curl);
	curl_easy_cleanup(curl);
	parsed_json = json_tokener_parse(buf);
	if(parsed_json == NULL) {
		fprintf(stderr, "error\n");
		return -1;
	}

	json_object_object_get_ex(parsed_json, "client_id", &json_client_id);
	json_object_object_get_ex(
		parsed_json, "client_secret", &json_client_secret);
	const char *client_id = json_object_get_string(json_client_id);
	const char *client_secret = json_object_get_string(json_client_secret);

	char *fmt = "%s%sresonse_type=code&client_id=%s&redirect_uri=urn:ietf:wg:"
			  "oauth:2.0:oob&force_login&scope=read write follow";
	api_url = "/oauth/authorize?";

	free(post_url);

	post_url = NULL;

	dm_asprintf(&post_url, fmt, instance, api_url, client_id);
	puts(post_url);

	curl = curl_easy_init();

	char *code = NULL;
	code = readline("Please log in in that url and paste the code here: ");

	char *access_token_fmt =
		"client_id=%s&client_secret=%s&redirect_uri=urn:ietf:wg:oauth:2.0:"
		"oob&grant_type=authorization_code&code=%s&scope=read write follow";
	api_url = "/oauth/token";

	char *post_token_url = NULL;

	dm_asprintf(
		&post_token_url, access_token_fmt, client_id, client_secret, code);

	post_url = NULL;

	dm_asprintf(&post_url, "%s%s", instance, api_url);

	curl_easy_setopt(curl, CURLOPT_URL, post_url);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_token_url);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, buf);
	curl_easy_perform(curl);

	parsed_json = json_tokener_parse(buf);
	json_object_object_get_ex(parsed_json, "access_token", &json_access_token);
	const char *access_token = json_object_get_string(json_access_token);
	store_config(instance, client_id, client_secret, access_token);

	free(code);
	free(post_url);
	free(post_token_url);
	return 0;
}
