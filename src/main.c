#include <stdio.h>
#include <readline/readline.h>
#include <unistd.h>

#include "login.h"
#include "util.h"
#include "post.h"

int
main(void)
{
	if(access(".demiurgerc", F_OK ))
		setup();
	char *post  = readline("Enter your post: ");
	char *scope = readline("Enter the visibility [public]: ");
	post_status(post,scope);
}
