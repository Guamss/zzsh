#pragma once

#include "../cmd/cmd.h"
#include "../../lib/bozolib/bozolib.h"

char* builtin_path(const char* executable);
int change_directory(char** args, lst** env);
int where(char** args);
int builtin_execute(cmd* input, lst** env);
