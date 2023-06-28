#include "env.h"
#include "../../lib/bozolib/bozolib.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

void env_del(void *ptr)
{
	env* content;

	content = ptr;
	free(content->key);
	free(content->value);
	free(content);
}

static env* create_env_content(const char *key, const char *value)
{
	env* out;
	char *key_dup;
	char *value_dup;

	out = malloc(sizeof(env));
	if (out == NULL)
		return (NULL);
	key_dup = strdup(key);
	if (key_dup == NULL)
	{
		free(out);
		return (NULL);
	}
	value_dup = strdup(value);
	if (value_dup == NULL)
	{
		free(key_dup);
		free(out);
		return (NULL);
	}
	out->key = key_dup;
	out->value = value_dup;
	return (out);
}

static lst* create_env(const char* key, const char* value)
{
	lst* out = malloc(sizeof(lst));
	if (out == NULL)
		return (NULL);
	out->content = create_env_content(key, value);
	if (out->content == NULL)
	{
		free(out);
		return (NULL);
	}
	out->next = NULL;
	return (out);
}

int add_env_variable(lst** root, const char* key, const char *value)
{
	lst* element;

	element = create_env(key, value);
	if (element == NULL)
		return 1;
	lst_addback(root, element);
	return 0;
}

lst** env_init(const char** env)
{
	lst** root;
	char* equal;
	char* key;
	char* value;

	root = malloc(sizeof(lst*));
	if (root == NULL)
		return (NULL);
	*root = NULL;
	for (size_t i = 0; env[i] != NULL; i++)
	{
		equal = strchr(env[i], '=');
		key = strndup(env[i], equal - env[i]);
		if (key == NULL)
		{
			lst_clear(root, &env_del);
			return (NULL);
		}
		value = strdup(env[i] + (equal - env[i]) + 1);
		if (value == NULL)
		{
			lst_clear(root, &env_del);
			free(key);
			return (NULL);
		}
		if (add_env_variable(root, key, value))
		{
			lst_clear(root, &env_del);
			free(value);
			free(key);
			return (NULL);
		}
	}
	return (root);
}

char *get_env_variable(lst** root, const char* key)
{
	lst* current = *root;
	env *content;

	while (current != NULL)
	{
		content = current->content;
		if (strcmp(content->key, key) == 0)
			return (content->value);
		current = current->next;
	}
	return (NULL);
}
