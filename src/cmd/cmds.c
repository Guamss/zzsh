#include "./cmd.h"
#include <stddef.h>
#include <stdlib.h>

lst** cmds_init(size_t len)
{
	lst** root;
	lst* current;
	cmd* content;

	root = calloc(sizeof(lst*), 1);
	if (root == NULL)
		return (NULL);
	for (size_t i = 0; i < len; i++)
	{
		current = malloc(sizeof(lst));
		if (current == NULL)
		{
			lst_clear(root, &free);
			free(root);
			return (NULL);
		}
		lst_addback(root, current);
		content = calloc(sizeof(cmd), 1);
		if (content == NULL)
		{
			lst_clear(root, &free);
			free(root);
			return (NULL);
		}
		current->content = content;
	}
	return (root);
}
