#pragma once

#include "../cmd/cmd.h"
#include "../../lib/bozolib/bozolib.h"
#include "../data/data.h"

int cmds_list_exec(lst** cmds, data_t *data);
int execute(cmd* input, lst** env);
int builtin_execute(cmd* input, data_t* data, int fd_int, int fd_out);
char* get_executable_path(const char* executable, lst** env);
