#include "utils.h"
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static size_t get_len(const char *str, const char* charset)
{
	size_t len = 0;
	size_t i = 0;

	while (str[i] != '\0')
	{
		while (str[i] != '\0' && (is_in_quote(str, i) || strchr(charset, str[i]) != NULL))
			i++;
		if (str[i] == '\0')
			return len;
		len++;
		while (str[i] != '\0' && (is_in_quote(str, i) || strchr(charset, str[i]) == NULL))
			i++;
	}
	return (len);
}

void free_tab(char **tab)
{
	for (size_t i = 0; tab[i] != NULL; i++)
		free(tab[i]);
	free(tab);
}

int fill_tab(char **tab, const char* str, const char* charset)
{
	const char *start;
	size_t len = 0;
	size_t i = 0;

	while (str[i] != '\0')
	{
		while (str[i] != '\0' && (is_in_quote(str, i) || strchr(charset, str[i]) != NULL))
			i++;
		if (str[i] == '\0')
			return 0;
		start = str + i;
		while (str[i] != '\0' && (is_in_quote(str, i) || strchr(charset, str[i]) == NULL))
			i++;
		tab[len] = strndup(start, (str + i) - start);
		if (tab[len] == NULL)
		{
			free_tab(tab);
			return 1;
		}
		len++;
	}
	return 0;
}

char **split_quoted_charset(const char *str, const char *charset)
{
	size_t len = get_len(str, charset);
	char **tab = malloc((len + 1) * sizeof(char *));
	if (tab == NULL)
		return NULL;
	tab[len] = NULL;
	if (fill_tab(tab, str, charset))
		return NULL;
	return (tab);
}
