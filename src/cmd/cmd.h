#pragma once

#include <stdlib.h>
#include <stddef.h>

#include "../../lib/bozolib/bozolib.h"

typedef struct s_cmd
{
	char *executable;
	char **args;
	int fd_in;
	int fd_out;
  int pid;
} cmd;

void cmd_del(void* ptr);

lst** cmds_init(size_t len);
