#include "./env/env.h"
#include <stdio.h>

int main(int ac, char **av, char **env)
{
	lst** lst_env;
	lst* current;
	struct s_env* content;
	(void) av;
	(void) ac;
	lst_env = env_init((const char **) env);
	if (lst_env == NULL)
		return (1);
	current = *lst_env;
	while (current != NULL)
	{
		content = current->content;
		if (content == NULL)
			printf("nil\n");
		printf("%s=%s\n", content->key, content->value);
		current = current->next;
	}
}
