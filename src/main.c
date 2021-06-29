#include <stdio.h>
#include <readline/readline.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <libgen.h>

#include "login.h"
#include "util.h"
#include "post.h"
#include "upload_file.h"

/* prints usage */

void
usage(const char *progname)
{
	printf("Usage: %s -s=status [-v=visibility] [-F=filename]\n", progname);
	return;
}

/* prints a string to stderr */

int
main(int argc, char **argv)
{
	if(access(".demiurgerc", F_OK))
		setup();
	int c;
	char *status = NULL;
	char *visibility = NULL;
	char *id_ptr = NULL;

	/* TODO: Support filename and visibility */

	if(!isatty(0)) {
		eputs("Not a terminal, reading from stdin");
		char *status = readline("");
		post_status(status, NULL, NULL);
		return 0;
	}

	if(argc == 1) {
		usage(argv[0]);
		return -1;
	}

	while((c = getopt(argc, argv, "s:v:F:")) != -1) {
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
}
