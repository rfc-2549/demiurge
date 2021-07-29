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

/* I am confident to present you, without a doubt, the worst code i've
 * ever written */

static int
get_client_id(struct config *config)
{
	char *api_url = "/api/v1/apps";

	CURL *curl = curl_easy_init();

	if(curl == NULL) {
		fprintf(stderr, "Error creating curl (what?)\n");
		return -1;
	}

	struct memory chunk = {0};
	
	char *post_url = NULL;
	dm_asprintf(&post_url, "%s%s", config->instance, api_url);
	curl_easy_setopt(curl, CURLOPT_URL, post_url);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cb);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

	/* TODO: Better way lol */
	/* If someone wants to do it, use curl_mime thing, I am to lazy
	 * to do this */

	curl_easy_setopt(curl,
		CURLOPT_POSTFIELDS,
		"client_name=demiurge"
		"&redirect_uris=urn:ietf:wg:oauth:2.0:oob"
		"&scope=read+write+follow"
		"&website=https://example.org");

	curl_easy_perform(curl);
	curl_easy_cleanup(curl);
	free(post_url);

	struct json_object *parsed_json;
	struct json_object *json_client_id;
	struct json_object *json_client_secret;
	parsed_json = json_tokener_parse(chunk.response);
	if(parsed_json == NULL) {
		fprintf(stderr, "error\n");
		free_response(&chunk);
		return -1;
	}

	json_object_object_get_ex(parsed_json, "client_id", &json_client_id);
	json_object_object_get_ex(parsed_json, "client_secret", &json_client_secret);

	const char *client_id = json_object_get_string(json_client_id);
	const char *client_secret = json_object_get_string(json_client_secret);
	int ret;

	if(!client_id || !client_secret) {
		eputs("Can't get client_id or client_secret");
		ret = -1;
	} else {
		dm_strncpy(config->client_id, client_id, sizeof(config->client_id));
		dm_strncpy(config->client_secret, client_secret, sizeof(config->client_secret));
		ret = 0;
	}

	free_response(&chunk);
	return ret;
}

static char *
ask_for_token(struct config *config)
{
	const char *api_url = "/oauth/authorize?";
	char *fmt = "%s%s"
		"client_id=%s"
		"&redirect_uri=urn:ietf:wg:oauth:2.0:oob"
		"&response_type=code"
		"&scope=read+write+follow\n";

	printf(fmt, config->instance, api_url, config->client_id);

	return readline("Please log in in that url and paste the code here: ");
}

static int
get_token(struct config *config, const char *code)
{
	char *fmt =
		"client_id=%s"
		"&client_secret=%s"
		"&redirect_uri=urn:ietf:wg:oauth:2.0:oob"
		"&code=%s"
		"&grant_type=authorization_code"
		"&scope=read+write+follow";
	const char *api_url = "/oauth/token";

	CURL *curl = curl_easy_init();
	if(curl == NULL) {
		fprintf(stderr, "Error creating curl (what?)\n");
		return -1;
	}

	struct memory chunk = {0};

	char *post_token_url;
	char *post_url;

	dm_asprintf(&post_token_url, fmt, config->client_id, config->client_secret, code);
	dm_asprintf(&post_url, "%s%s", config->instance, api_url);

	curl_easy_setopt(curl, CURLOPT_URL, post_url);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_token_url);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cb);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

	curl_easy_perform(curl);
	curl_easy_cleanup(curl);
	free(post_token_url);
	free(post_url);

	struct json_object *parsed_json;
	struct json_object *json_access_token;
	
	parsed_json = json_tokener_parse(chunk.response);
	json_object_object_get_ex(parsed_json, "access_token", &json_access_token);
	const char *access_token = json_object_get_string(json_access_token);
	int ret;
	if(!access_token) {
		eputs("Can't get access_token");
		ret = -1;
	} else {
		dm_strncpy(config->access_token, access_token, sizeof(config->access_token));
		ret = 0;
	}

	free_response(&chunk);

	return ret;
}

int
setup()
{
	struct config config;

	dm_strncpy(config.instance, readline("Enter your instance (e.g. https://social.fnord.tld) "),
		sizeof(config.instance));

	if(get_client_id(&config) < 0)
		return -1;

	char *code = ask_for_token(&config);

	if(get_token(&config, code) < 0)
	{
		free(code);
		return -1;
	}

	store_config(&config);

	free(code);
	return 0;
}
