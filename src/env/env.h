#pragma once

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../lib/bozolib/bozolib.h"
#include "../data/data.h"
#include "../utils/utils.h"

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
char* interpret_env_var(data_t*, const char*);
char** get_env_str(lst** root);
