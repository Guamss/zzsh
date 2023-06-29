#include "utils.h"

static void	remove_quote_and_reset(char *str, ssize_t *start, ssize_t *stop)
{
	str_shift(str + *start, -1);
	str_shift(str + *stop - 1, -1);
	*start = -1;
	*stop = -1;
}

void quote_remover(char *str)
{
	size_t	i;
	ssize_t	start;
	ssize_t	stop;

	start = -1;
	stop = -1;
	i = 0;
	while (str[i] != '\0')
	{
		if ((str[i] == '\"' || str[i] == '\''))
		{
			if (start == -1)
				start = i;
			else if (str[i] == str[start])
				stop = i;
		}
		if (stop != -1)
		{
			remove_quote_and_reset(str, &start, &stop);
			i = i - 1;
		}
		else
			i++;
	}
}

