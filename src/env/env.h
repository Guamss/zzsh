#pragma once
#include "../../lib/bozolib/bozolib.h"

typedef struct s_env
{
	char *key;
	char *value;
} env;

lst** env_init(const char **env);
void env_del(void *ptr);

char *get_env_variable(lst** root, const char* key);
int add_env_variable(lst** root, const char *key, const char *value);
int edit_env_variable(lst** root, const char *key, const char *new_value);
