#include "parsing.h"

char* parsing_executable(const char* executable, data_t *data)
{
	if (executable == NULL)
		return NULL;
	if (strchr("./", executable[0]))
		return strdup(executable);
	return (get_executable_path(executable, data->env));
}

int parsing_cmd(char *str, cmd* command, data_t *data)
{
	char* tmp;
	if (get_redirections(str, command))
		return 1;
	command->args = split_quoted_charset(str, "\t ");
	if (command->args == NULL)
		return 1;
	for (size_t i = 0; command->args[i]; i++)
		quote_remover(command->args[i]);
	if (command->args[0][0] == '\\')
	{
		tmp = strdup(command->args[0] + 1);
		if (tmp != NULL)
		{
			free(command->args[0]);
			command->args[0] = strdup(tmp);
		}
	}
	else
	{
		tmp = get_alias(data->aliases, command->args[0]);
		if (tmp != NULL)
		{
			free(command->args[0]);
			command->args[0] = strdup(tmp);
		}
	}
	if (command->args[0] == NULL)
		return 1;
	command->executable = parsing_executable(command->args[0], data);
	return 0;
}

lst **parsing_pipe(const char *str, data_t *data)
{
	char** cmds_str;
	lst** cmds;
	lst* current;

	cmds_str = split_quoted_charset(str, "|");
	if (cmds_str == NULL)
		return (NULL);
	cmds = cmds_init(tablen((const void**)cmds_str));
	if (cmds == NULL)
	{
		tab_free((void**)cmds_str);
		return (NULL);
	}
	current = *cmds;
	for (size_t i = 0; cmds_str[i] != NULL; i++)
	{
		if (parsing_cmd(cmds_str[i], current->content, data))
		{
			tab_free((void**)cmds_str);
			return NULL;
		}
		current = current->next;
	}
	tab_free((void**)cmds_str);
	return (cmds);
}

lst*** parsing(const char *line, data_t *data)
{
	char** line_commas;
	lst*** tab;

	line_commas = split_quoted_charset(line, ";");
	if (line_commas == NULL)
		return (NULL);
	tab = malloc(sizeof(lst**) * (tablen((const void**) line_commas) + 1));
	if (tab == NULL)
	{
		tab_free((void **) line_commas);
		return (NULL);
	}
	size_t i;
	for (i = 0; line_commas[i] != NULL; i++)
	{
		tab[i] = parsing_pipe(line_commas[i], data);
		if (tab[i] == NULL)
		{
			tab_free((void**) line_commas);
			return (NULL);
		}
	}
	tab_free((void **) line_commas);
	tab[i] = NULL;
	return (tab);
}

