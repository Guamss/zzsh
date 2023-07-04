#include "./builtin.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>

static void print_aliases(lst** aliases, int fd_out)
{
	lst* current = *aliases;
	alias_t* content;

	while (current != NULL)
	{
		content = current->content;
		dprintf(fd_out, "%s='%s'\n", content->key, content->value);
		current = current->next;
	}
}
int builtin_alias(data_t* data, cmd_t* cmd)
{
	const char* equal;
	char* result;
	char* key;
	if (tablen((const void**) cmd->args) == 1)
		print_aliases(data->aliases, cmd->output[0]);
	for (size_t i = 1; cmd->args[i] != NULL; i++)
	{
		equal = strchr(cmd->args[i], '=');
		if (equal == NULL)
		{
			result = get_alias(data->aliases, cmd->args[i]);
			if (result != NULL)
				dprintf(cmd->output[0], "%s=%s\n", cmd->args[i], result);
		}
		else
		{
			key = strndup(cmd->args[i], equal - cmd->args[i]);
			if (key == NULL)
				return 1;
			if (add_alias(data->aliases, key, equal + 1))
			{
				free(key);
				return 1;
			}
			free(key);
		}
	}
	return 0;
}
