#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <limits.h>
#include <unistd.h>
#include "../../lib/bozolib/bozolib.h"
#include "../../lib/bozolib/src/str/str.h"
#include "../env/env.h"
#include <fcntl.h>

static char* get_prompt(lst** env)
{
	char* out;
	char cwd[PATH_MAX];
	if (getcwd(cwd, sizeof(cwd)) == NULL)
		return NULL;
  char hostname_buff[BUFSIZ];
  int file_host = open("/etc/hostname", O_RDONLY);
  if (file_host == -1)
    strcpy(hostname_buff, "");
  read(file_host, hostname_buff, BUFSIZ);
  close(file_host);
  char* first_line_return = strchr(hostname_buff, '\n');
  if (first_line_return)
  {
    *first_line_return = '\0';
  }
  char* user = get_env_variable(env, "USER");
	char* home = get_env_variable(env, "HOME");
	if (strncmp(home, cwd, strlen(home)) == 0)
	{
		strcpy(cwd, "~");
		strcat(cwd, cwd + strlen(home));
	}  
  out = str_merger(8, "[", user, "@", hostname_buff, "]", " > ", cwd, " ");
  if (strcmp(user, "root") == 0)
    out = str_merger(2, out, "# ");
  else
    out = str_merger(2, out, "$ ");
  return out;
}

char *get_user_input(lst** env)
{
	char *prompt;
	char *input;
	
	prompt = get_prompt(env);
	if (prompt == NULL)
		return NULL;
	input = readline(prompt);
	free(prompt);
	if (input == NULL)
		printf("exit");
	if (input != NULL && str_contain_only(input, "\t ") == 0)
		add_history(input);
	return (input);
}
