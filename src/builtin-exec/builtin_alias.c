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
int builtin_alias(int fd_in, int fd_out, lst** aliases, char **args)
{
	const char* equal;
	char* result;
	char* key;
	(void)fd_in;
	if (tablen((const void**) args) == 1)
		print_aliases(aliases, fd_out);
	for (size_t i = 1; args[i] != NULL; i++)
	{
		equal = strchr(args[i], '=');
		if (equal == NULL)
		{
			result = get_alias(aliases, args[i]);
			if (result != NULL)
				dprintf(fd_out, "%s=%s\n", args[i], result);
		}
		else
		{
			key = strndup(args[i], equal - args[i]);
			if (key == NULL)
				return 1;
			if (add_alias(aliases, key, equal + 1))
			{
				free(key);
				return 1;
			}
			free(key);
		}
	}
	return 0;
}
