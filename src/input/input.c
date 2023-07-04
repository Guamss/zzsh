#include "./input.h"
#include <readline/readline.h>

static char* get_prompt(data_t *data)
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
  char* user = get_env_variable(data->env, "USER");
	char* home = get_env_variable(data->env, "HOME");
	if (strncmp(home, cwd, strlen(home)) == 0)
	{
		strcpy(cwd, "~");
		strcat(cwd, cwd + strlen(home));
	}  
  out = str_merger(8, user, "[", "@", hostname_buff, "]", " > ", cwd, " ");
  char* temp = out;
  if (strcmp(user, "root") == 0)
    out = str_merger(2, "# ", out);
  else
    out = str_merger(2,  "$ ", out);
  free(temp);
  return out;
}

static void finisher(char** strs)
{
	char* tmp;
	char* tmp2;
	(void)tmp2;
	if (strs[0] == NULL)
		return;
	while (strs[0] != NULL && is_in_quote(strs[0], 0 - 1))
	{
		tmp2 = readline("> ");
		if (tmp2 == NULL)
			return;
		tmp = str_merger(3, "\n", strs[0], tmp2);
		free(strs[0]);
		if (tmp == NULL)
		{
			strs[0] = NULL;
			return;
		}
		strs[0] = tmp;
	}
}

char *get_user_input(data_t *data)
{
	char *prompt;
	char *input;
	
	prompt = get_prompt(data);
	if (prompt == NULL)
		return NULL;
	input = readline(prompt);
	free(prompt);
	finisher(&input);
	if (input == NULL)
		printf("exit");
	if (input != NULL && str_contain_only(input, "\t ") == 0)
		add_history(input);
	return (input);
}
