#pragma once

#include "../../lib/bozolib/bozolib.h"

typedef struct s_data
{
	const char* computor_name;
	int			status_code;
	lst**		env;
	lst**		aliases;
} data_t;
