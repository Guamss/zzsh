#pragma once

#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>

#include "../../lib/bozolib/bozolib.h"

typedef struct s_cmd
{
	char *executable;
	char **args;
	int input[2];
	int output[2];
	int pid;
} cmd_t;

void cmd_del(void* ptr);
int cmd_init(cmd_t* command);
void cmd_close(void* ptr);
