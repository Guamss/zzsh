#include "./cmd.h"

void cmd_del(void *ptr)
{
	cmd* content = ptr;

	tab_free((void**)content->args);
	free(content->executable);
}
