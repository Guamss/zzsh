#pragma once

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <limits.h>
#include <fcntl.h>

#include "../../lib/bozolib/bozolib.h"
#include "../utils/utils.h"
#include "../env/env.h"
#include "../data/data.h"

char *get_user_input(data_t *data);
