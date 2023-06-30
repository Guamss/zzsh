#pragma once

#include "../cmd/cmd.h"
#include "../../lib/bozolib/bozolib.h"

int cmds_list_exec(lst** cmds, lst** env);
int execute(cmd* input, lst** env);
int builtin_execute(cmd* input, lst** env);
int len(void** list);
char* get_executable_path(const char* executable, lst** env);
