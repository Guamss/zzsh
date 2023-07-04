#include "./redirection.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

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

void redirection_fill(const char* str, size_t nb_symbols, cmd_t* command, const char *path)
{
	int fd;
	if (str[0] == '>')
	{
		if (nb_symbols == 1)
			fd = open(path, O_TRUNC | O_CREAT | O_WRONLY, 0644);
		else
			fd = open(path, O_APPEND | O_CREAT | O_WRONLY, 0644);
		if (command->output[0] > 2)
			close(command->output[0]);
		if (fd == -1)
		{
			dprintf(2, "zzsh: permission denied: %s\n", path);
			fd = -2;
		}
		command->output[0] = fd;
	}
	else
	{
		if (access(path, F_OK))
			dprintf(2, "zzsh: no such file or directory: %s\n", path);
		else if (access(path, R_OK))
			dprintf(2, "zzsh: permission denied: %s\n", path);
		if (nb_symbols == 1)
			fd = open(path, O_RDONLY);
		else
			fd = open(path, O_RDONLY);
		if (command->output[0] > 2)
			close(command->output[0]);
		if (fd == -1)
			fd = -2;
		command->output[0] = fd;
	}
}

static size_t get_concecutive(const char *str)
{
	size_t i;
	for (i = 1; str[i] == str[0]; i++);
	return (i);
}

int get_redirections(char *str, cmd_t* command)
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
				redirection = strchr(redirection + 2, redirection_symbol[i]);
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
				redirection_fill(redirection, redirection_type, command, path);
				str_shift(redirection, (get_path_size(redirection + redirection_type) + redirection_type) * -1);
				free(path);
			}
			redirection = redirection + redirection_type;
		}
	}
	return (0);
}
