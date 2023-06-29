#pragma once
#include <stddef.h>
#include <string.h>

int is_in_quote(const char *str, size_t pos);
char **split_quoted_charset(const char *str, const char *charset);
