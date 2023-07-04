#pragma once

#include <stdio.h>
#include <limits.h>
#include <unistd.h>

#include "../env/env.h"
#include "../cmd/cmd.h"
#include "../data/data.h"
#include "../alias/alias.h"
#include "../exec/exec.h"
#include "../../lib/bozolib/bozolib.h"

char* builtin_path(const char* executable);
int builtin_execute(data_t* data, cmd_t* cmd);

int builtin_alias(data_t* data, cmd_t* cmd);
int change_directory(char** args, lst** env);
int where(char** args, lst** env);

