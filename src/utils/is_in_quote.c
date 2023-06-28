#include "utils.h"

int is_in_quote(const char *str, size_t pos)
{
	int out = 0;

	for (size_t i = 0; str[i] != '\0' && i < pos; i++)
	{
		if (str[i] == '\"')
		{
			if (out == 2)
				out = 0;
			else
				out = 2;
		}
		else if (str[i] == '\'')
		{
			if (out == 1)
				out = 0;
			else
				out = 1; 
		}
	}
	return (out);
}
