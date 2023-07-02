#include "./redirection.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>

char* get_path(const char* start_path)
{
	size_t i, j;
	char* out;

	i = 0;
	while (strchr(" \t", start_path[i]))
		i++;
	j = i;
	while (start_path[j] != '\0' && strchr(" \t", start_path[j]) == NULL) 
		j++;
	if (str_contain_only(start_path + i, "\t "))
		return NULL;
	out = strndup(start_path + i, j);
	if (out == NULL)
	{
		dprintf(2, "zzsh: malloc failed\n");
		return NULL;
	}
	quote_remover(out);
	return out;
}

static size_t get_path_size(const char* str)
{
	size_t i = 0;
	while (strchr("\t ", str[i]) != NULL)
		i++;
	while (str[i] != '\0' && strchr("\t ", str[i]) == NULL)
		i++;
	return i;
}

int redirection_fill(const char* str, size_t nb_symbols, cmd* command, const char *path)
{
	int problem = 0;
	if (str[0] == '>')
	{
		if (command->fd_out > 2)
			close(command->fd_out);
		if (nb_symbols == 1)
			command->fd_out = open(path, O_TRUNC | O_CREAT | O_WRONLY, 0644);
		else
			command->fd_out = open(path, O_APPEND | O_CREAT | O_WRONLY, 0644);
		problem = command->fd_out == -1;
	}
	else
	{
		if (command->fd_in > 2)
			close(command->fd_in);
		if (nb_symbols == 1)
			command->fd_in = open(path, O_RDONLY);
		else
			command->fd_in = open(path, O_RDONLY);
		problem = command->fd_in == -1;
	}
	if (problem)
		dprintf(2, "zzsh: \"%s\": file error\n", path);
	return problem;
}

static size_t get_concecutive(const char *str)
{
	size_t i;
	for (i = 1; str[i] == str[0]; i++);
	return (i);
}

int get_redirections(char *str, cmd* command)
{
	char *redirection;
	char redirection_symbol[3] = "<>";
	char *path;
	size_t redirection_type;

	for (size_t i = 0; redirection_symbol[i] != '\0'; i++)
	{
		redirection = str;
		while (redirection != NULL)
		{
			redirection = strchr(redirection, redirection_symbol[i]);
			while (redirection != NULL && is_in_quote(str, redirection - str))
				redirection = strchr(redirection, '>');
			if (redirection == NULL)
				break;
			redirection_type = get_concecutive(redirection);
			if (redirection_type > 2)
			{
				dprintf(2, "zzsh: invalid redirection type\n");
				return 1;
			}
			else
			{
				path = get_path(redirection + redirection_type);
				if (path == NULL)
					return 1;
				if (redirection_fill(redirection, redirection_type, command, path))
				{
					free(path);
					return (1);
				}
				str_shift(redirection, (get_path_size(redirection + redirection_type) + redirection_type) * -1);
				free(path);
			}
			redirection = redirection + redirection_type;
		}
	}
	return (0);
}
