#pragma once

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "../../lib/bozolib/bozolib.h"

int is_in_quote(const char *str, size_t pos);
void quote_remover(char *str);
char **split_quoted_charset(const char *str, const char *charset);
