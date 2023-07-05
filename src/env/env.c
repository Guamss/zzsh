#include "env.h"
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
		free(key);
		free(value);
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

int edit_env_variable(lst** root, const char *key, const char *new_value)
{
	lst* current = *root;
	env* content;
	
	while (current != NULL)
	{
		content = current->content;
		if (strcmp(content->key, key) == 0)
		{
			free(content->value);
			content->value = strdup(new_value);
			if (content->value == NULL)
				return (2);
			return (0);
		}
		current = current->next;
	}
	return (1);
}

char **get_env_str(lst** root)
{
	lst* current = *root;
	env* content;
	char** tab;
	size_t i;
	size_t len;
	
	tab = malloc(sizeof(char*) * (lst_len(*root) + 1));
	if (tab == NULL)
		return (NULL);
	i = 0;
	while (current != NULL)
	{
		content = current->content;
		len = strlen(content->key) + 1 + strlen(content->value) + 1;
		tab[i] = malloc(len * sizeof(char));
		if (tab[i] == NULL)
		{
			tab_free((void**) tab);
			return NULL;
		}
		strcpy(tab[i], content->key);
		strcat(tab[i], "=");
		strcat(tab[i], content->value);
		current = current->next;
		i++;
	}
	tab[i] = NULL;
	return tab;
}

static char* get_key(const char* key_start)
{
	if (strncmp(key_start, "~", 1) == 0)
		return strdup("~");
	size_t i = 1;
	while (key_start[i] != '\0' && strchr("\t $\"\'~", key_start[i]) == NULL)
		i++;
	return (strndup(key_start, i));
}

static char* get_value(data_t* data, const char* key)
{
	char *out;
	if (strcmp(key, "~") == 0)
		out = get_env_variable(data->env, "HOME");
	else if (strcmp(key, "$?") == 0)
		out = itoA(data->status_code);
	else
		out = get_env_variable(data->env, key + 1);
	if (out == NULL)
		out = "";
	return out;
}

char* interpret_env_var(data_t *data, const char* str)
{
	char symbols[3] = "$~";
	char* out;
	char* key;
	char* value;
	char* tmp2;
	const char* tmp;

	out = strdup(str);
	if (out == NULL)
		return NULL;
	for (size_t i = 0; symbols[i] != '\0'; i++)
	{
		tmp = strchr(out,symbols[i]);
		while (tmp != NULL && is_in_quote(tmp, str - tmp) == 1)
			tmp = strchr(tmp, symbols[i]);
		while (tmp != NULL)
		{
			if (tmp == NULL)
				continue;
			key = get_key(tmp);
			if (key == NULL)
			{
				free(out);
				return NULL;
			}
			value = get_value(data, key);
			if (value == NULL)
			{
				free(key);
				free(out);
				return NULL;
			}
			tmp2 = str_replace(out, value, tmp - out, tmp - out + strlen(key));
			free(key);
			free(out);
			out = tmp2;
			tmp = strchr(out,symbols[i]);
			while (tmp != NULL && is_in_quote(tmp, str - tmp) == 1)
				tmp = strchr(tmp, symbols[i]);
		}
	}
	return out;
}
