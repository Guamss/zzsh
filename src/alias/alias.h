#pragma once

#include "../../lib/bozolib/bozolib.h"

typedef struct s_alias
{
	char* key;
	char* value;
} alias_t;

lst** aliases_init();
lst** aliases_save(lst** aliases);
char* get_alias(lst** aliases, const char* key);
int add_alias(lst** root, const char* key, const char* value);
