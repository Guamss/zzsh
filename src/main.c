#include "prompt/prompt.h"
#include "./env/env.h"
#include <stdio.h>

int main(int ac, char **av, char **env)
{
	lst** lst_env;
	(void) av;
	(void) ac;
	lst_env = env_init((const char **) env);
  printf(get_prompt(lst_env));
}
