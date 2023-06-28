#include "./env/env.h"
#include "./input/input.h"
#include <stdio.h>
#include "cmd/cmd.h"
#include "./exec/exec.h"

int main(int ac, char **av, char **env_str)
{
	char *line;
	lst** env;

	(void) av;
	(void) ac;
	env = env_init((const char **) env_str);
	if (env == NULL)
		return (1);
	line = get_user_input(env);
	while (line != NULL)
	{
		free(line);
		line = get_user_input(env);
	}
	lst_clear(env, &env_del);
	return (0);
 
}
