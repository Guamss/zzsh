#include "./cmd.h"

void cmd_del(void *ptr)
{
	cmd_t* content = ptr;

	tab_free((void**)content->args);
	free(content->executable);
	free(content);
}

int cmd_init(cmd_t* command)
{
	command->pid = -1;
	command->input[0] = -1;
	command->input[1] = -1;
	command->output[0] = -1;
	command->output[1] = -1;
	return 0;
}

void cmd_close(void* ptr)
{
	cmd_t* content = ptr;
	if (content->input[0] > 2)
		close(content->input[0]);
	// content->input[0] = -1;
	if (content->input[1] > 2)
		close(content->input[1]);
	// content->input[1] = -1;
	if (content->output[0] > 2)
		close(content->output[0]);
	// content->output[0] = -1;
	if (content->output[1] > 2)
		close(content->output[1]);
	// content->output[1] = -1;
}
