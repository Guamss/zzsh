#pragma once
#include "../../lib/bozolib/bozolib.h"

typedef struct s_env
{
	char *key;
	char *value;
} env;

int add_env_variable(lst** root, const char *key, const char *value);
lst** env_init(const char **env);
void env_del(void *ptr);
char *get_env_variable(lst** root, const char* key);
