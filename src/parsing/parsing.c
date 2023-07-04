#include "parsing.h"

char* parsing_executable(const char* executable, data_t *data)
{
	if (executable == NULL)
		return NULL;
	if (strchr("./", executable[0]))
		return strdup(executable);
	return (get_executable_path(executable, data->env));
}

int parsing_cmd(char *str, cmd_t* command, data_t *data)
{
	char* tmp;
	if (cmd_init(command))
		return 1;
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
	cmds = malloc(sizeof(lst*));
	if (cmds == NULL)
	{
		tab_free((void**)cmds_str);
		return (NULL);
	}
	if (cmds_str[0] != NULL)
	{
		*cmds = malloc(sizeof(lst));
		if (*cmds == NULL)
		{
			free(cmds);
			tab_free((void**)cmds_str);
			return (NULL);
		}
		current = *cmds;
	}
	for (size_t i = 0; cmds_str[i] != NULL; i++)
	{
		current->content = malloc(sizeof(cmd_t));
		if (current->content == NULL)
		{
			tab_free((void**)cmds_str);
			lst_clear(cmds, &cmd_del);
			return NULL;
		}
		if (parsing_cmd(cmds_str[i], current->content, data))
		{
			tab_free((void**)cmds_str);
			lst_clear(cmds, &cmd_del);
			return NULL;
		}
		if (cmds_str[i + 1] != NULL)
		{
			current->next = malloc(sizeof(lst));
			if (current->next == NULL)
			{
				tab_free((void**)cmds_str);
				lst_clear(cmds, &cmd_del);
				return NULL;
			}
		}
		else
			current->next = NULL;
		if (i == 0)
			if (((cmd_t*) current->content)->input[0] == -1)
				((cmd_t*) current->content)->input[0] = 0;
		if (current->next == NULL)
			if (((cmd_t*) current->content)->output[0] == -1)
				((cmd_t*) current->content)->output[0] = 1;
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

