#pragma once

#include <stddef.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "../../lib/bozolib/bozolib.h"
#include "../cmd/cmd.h"
#include "../utils/utils.h"

int get_redirections(char *str, cmd_t* command);
