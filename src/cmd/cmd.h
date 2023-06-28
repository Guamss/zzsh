#pragma once

typedef struct s_cmd
{
	char *executable;
	char **args;
	int fd_in;
	int fd_out;
  int pid;
} cmd;
