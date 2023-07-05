#pragma once

#include "../exec/exec.h"
#include "../../lib/bozolib/bozolib.h"
#include "../redirection/redirection.h"
#include "../data/data.h"
#include "../alias/alias.h"
#include "../env/env.h"

lst*** parsing(const char* str, data_t *data);
