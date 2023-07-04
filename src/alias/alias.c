#include "../../lib/bozolib/bozolib.h"
#include "./alias.h"
#include <stdlib.h>

lst** aliases_init()
{
	lst** aliases;

	aliases = malloc(sizeof(lst*));
	*aliases = NULL;
	return aliases;
}

int add_alias(lst** root, const char* key, const char* value)
{
	lst* nouveau;
	alias_t* alias;

	alias = malloc(sizeof(alias_t));
	if (alias == NULL)
		return 1;
	nouveau = malloc(sizeof(lst));
	if (nouveau == NULL)
	{
		free(alias);
		return 1;
	}
	alias->key = strdup(key);
	if (alias->key == NULL)
	{
		free(alias->key);
		free(alias);
		return 1;
	}
	alias->value = strdup(value);
	if (alias->key == NULL)
	{
		free(alias->key);
		free(alias->value);
		free(alias);
		return 1;
	}
	nouveau->content = alias;
	lst_addback(root, nouveau);
	return 0;
}

char* get_alias(lst** aliases, const char* key)
{
	lst* current = *aliases;
	alias_t* content;

	while (current != NULL)
	{
		content = current->content;
		if (strcmp(key, content->key) == 0)
			return content->value;
		current = current->next;
	}
	return NULL;
}
