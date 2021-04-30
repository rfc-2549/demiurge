#include <stdio.h>
#include <readline/readline.h>
#include <unistd.h>

#include "login.h"
#include "util.h"
#include "post.h"

int
main(void)
{
	char client_id[50];
	char client_secret[50];
	char access_token[50];
	if(access(".demiurgerc", F_OK ))
		setup();
	post_status(readline("Enter your post: "),"public");
}
