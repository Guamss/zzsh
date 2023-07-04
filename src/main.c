#include "./env/env.h"
#include "./input/input.h"
#include <stddef.h>
#include <stdio.h>
#include "alias/alias.h"
#include "cmd/cmd.h"
#include "./exec/exec.h"
#include "./parsing/parsing.h"
#include "./data/data.h"

void cmds_list_destroyer(lst*** cmds_list)
{
	for (size_t i = 0; cmds_list[i] != NULL; i++)
		lst_clear(cmds_list[i], &cmd_del);
	free(cmds_list);
}

int main(int ac, char **av, char **env_str)
{
	char *line;
	data_t data;
	lst*** cmds_list;

	(void) av;
	(void) ac;
	data.env = env_init((const char **) env_str);
	if (data.env == NULL)
		return (1);
	data.aliases = aliases_init();
	if (data.aliases == NULL)
	{
		lst_clear(data.env, &env_del);
		return 1;
	}
	line = get_user_input(&data);
	while (line != NULL)
	{
		cmds_list = parsing(line, &data);
		free(line);
		if (cmds_list != NULL)
		{
			for (size_t i = 0; cmds_list[i] != NULL; i++)
			{
				if (cmds_list_exec(cmds_list[i], &data))
				{
					lst_clear(data.env, &env_del);
					return 1;
				}
			}
			cmds_list_destroyer(cmds_list);
		}
		line = get_user_input(&data);
	}
	lst_clear(data.aliases, alias_del);
	lst_clear(data.env, &env_del);
	return (0);
 
}
