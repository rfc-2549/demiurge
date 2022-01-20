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
#include <readline/readline.h>
#include <stdlib.h>
#include <getopt.h>
#include <libgen.h>
#include <stdbool.h>
#include <unistd.h>

#include "login.h"
#include "util.h"
#include "post.h"
#include "upload_file.h"
#include "follow.h"

/* prints usage */

void
usage(const char *progname)
{
	printf("Usage: %s -s=status [-v=visibility] [-F=filename]\n", progname);
	return;
}

void
help()
{
	puts("-s, --status: status to post");
	puts("-F, --filename: filename to be attatched with the post");
	puts("-v, --visibility: The visibility which the post will have, can"
		"be: public unlisted, private and direct");
	puts("-f, --follow: Follow an account");
	puts("-u, --unfollow: Unfollow an account");
	puts("-U, --usage: prints usage message");
	puts("-h, --help: prints this help message");
}

/* prints a string to stderr */

int
main(int argc, char **argv)
{
	if(config_exists() != 0 && setup() != 0) {
		eputs("Login failed");
	}
	int c;
	char *status = NULL;
	char *visibility = NULL;
	char *id_ptr = NULL;
	char *account_id = NULL;
	/* TODO: Support filename and visibility */

	if(!isatty(0)) {
		eputs("Not a terminal, reading from stdin");
		char status[5000]; /* TODO get max from instance */
		fread(status, 5000, 1, stdin);
		post_status(status, NULL, NULL);
		return 0;
	}

	if(argc == 1) {
		usage(argv[0]);
		return -1;
	}

	int option_index = 0;
	static struct option long_options[] = {
		{ "status", required_argument, 0, 's' },
		{ "filename", required_argument, 0, 'F' },
		{ "visibility", required_argument, 0, 'v' },
		{ "follow", required_argument, 0, 'f' },
		{ "unfollow", required_argument, 0, 'u' },
		{ "usage", no_argument, 0, 'U' },
		{ "help", no_argument, 0, 'h' },
		{ 0, 0, 0, 0 }
	};

	while((c = getopt_long(
			  argc, argv, "s:v:F:f:u:hU", long_options, &option_index)) !=
		 -1) {
		switch(c) {
			case 's':
				status = optarg;
				break;
			case 'v':
				visibility = optarg;
				break;
			case 'F':
				upload_file(optarg, basename(optarg), &id_ptr);
				break;
			case 'f':
				account_id = get_account_id(optarg);
				if(account_id) {
					follow_account(account_id, 'f');
					free(account_id);
				}
				return 0;
			case 'u':
				account_id = get_account_id(optarg);
				if(account_id) {
					follow_account(account_id, 'u');
					free(account_id);
				}
				return 0;
			case 'h':
				help();
				return 0;
				break;
			case 'U':
				usage(argv[0]);
				return 0;
				break;
			case '?':
				break;
			default:
				fprintf(stderr, "Unrecognized option: %c\n", c);
				return 0;
				break;
		}
	}

	if(status == NULL) {
		eputs("Enter a status (-s)");
		return -1;
	}

	if(visibility == NULL) {
		visibility = "public";
	}

	post_status(status, visibility, id_ptr);

	return 0;
}
