#pragma once

#include "../cmd/cmd.h"
#include "../../lib/bozolib/bozolib.h"
#include "../data/data.h"

int cmds_list_exec(lst** cmds, data_t *data);
char* get_executable_path(const char* executable, lst** env);
