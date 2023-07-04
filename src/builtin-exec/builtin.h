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
int builtin_execute(cmd_t* input, data_t* data, int fd_in, int fd_out);

int builtin_alias(int fd_in, int fd_out, lst** aliases, char** args);
int change_directory(char** args, lst** env);
int where(char** args, lst** env);

