#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <limits.h>
#include <unistd.h>
#include "../../lib/bozolib/bozolib.h"
#include "../env/env.h"
#include <fcntl.h>

static char* get_prompt(lst** env)
{
	char* out;
	int size;
	char cwd[PATH_MAX];
	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		dprintf(2, "chemin inconnu");
		return NULL;
	}
  char hostname_buff[BUFSIZ];
  int file_host = open("/etc/hostname", O_RDONLY);
  if (file_host == -1)
    strcpy(hostname_buff, "");
  read(file_host, hostname_buff, BUFSIZ);
  close(file_host);
  char* contains = strchr(hostname_buff, '\n');
  if (contains)
  {
    *contains = '\0';
  }
  char* user = get_env_variable(env, "USER");
	char* home = get_env_variable(env, "HOME");
	if (user == NULL)
		user = "";
	else if (strncmp(home, cwd, strlen(home)) == 0)
	{
		strcpy(cwd, "~");
		strcat(cwd, cwd + strlen(home));
	}
  if (strcmp(user, "root") == 0)
  {
    size = snprintf(NULL, 0, "[%s@%s] > %s # ",user, hostname_buff, cwd) + 10;
    out = malloc(size*sizeof(char));
    if(out == NULL)
      return NULL;
    sprintf(out, "[%s@%s] > %s # ",user, hostname_buff, cwd);
    return out;
  }
  else
  {
    size = snprintf(NULL, 0, "[%s@%s] >%s $ ",user, hostname_buff, cwd) + 10;
    out = malloc(size*sizeof(char));
    if(out == NULL)
      return NULL;
    sprintf(out, "[%s@%s] > %s $ ",user, hostname_buff, cwd);
    return out;
  }
}

char *get_user_input(lst** env)
{
	char *prompt;
	char *input;
	
	prompt = get_prompt(env);
	if (prompt == NULL)
		return NULL;
	input = readline(prompt);
	if (input != NULL && str_contain_only(input, "\t ") == 0)
		add_history(input);
	free(prompt);
	if (input == NULL)
		printf("exit");
	return (input);
}
