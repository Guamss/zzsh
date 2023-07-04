#include <sys/wait.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../cmd/cmd.h"
#include <unistd.h>
#include "../env/env.h"
#include "../utils/utils.h"
#include "../../lib/bozolib/bozolib.h"
#include "../builtin-exec/builtin.h"
#include "../data/data.h"

int execute(lst** cmds, cmd_t* cmd, lst** env)
{
  int pid;
  if (cmd->executable != NULL)
    pid = fork();
  else
  {
    dprintf(2, "Executable inconnu\n");
    return 1;
  }
  if (pid == -1)
	  return 1;
  else if (pid == 0)
  {
	char** env_str = get_env_str(env);
	if (env_str == NULL)
		return 1;
	dup2(cmd->output[0], 1);
	dup2(cmd->input[0], 0);
	lst_iter(cmds, &cmd_close);
  	execve(cmd->executable, cmd->args, env_str);
	dprintf(2, "execve failed\n");
	free((void**)env_str);
	return 1;
  }
  else
  {
	cmd->pid = pid;
  }
  return 0;
}

char* get_executable_path(const char* executable, lst** env)
{
  char* builtin = builtin_path(executable);
  if (builtin != NULL)
    return builtin;
  int size_path_str;
  char * path_file;
  char* path_env = get_env_variable(env, "PATH");
  char** path_env_splited =split_quoted_charset(path_env, ":");
  
  for (int i=0; path_env_splited[i] != NULL; i++)
  {
    size_path_str = strlen(path_env_splited[i])+strlen(executable)+1; 
    path_file = malloc((size_path_str+1)*sizeof(char));
    strcpy(path_file, path_env_splited[i]);
    strcat(path_file, "/");
    strcat(path_file, executable);
    if (access(path_file, X_OK) == 0)
    {
      tab_free((void**)path_env_splited);
      return path_file;
    }
    
    free(path_file);
  }
  tab_free((void**)path_env_splited);
  return NULL;
}

void add_fd(int fds[2], int fd)
{
	if (fds[0] == -1)
		fds[0] = fd;
	else
		fds[1] = fd;
}

int cmds_list_exec(lst** cmds, data_t *data)
{
	lst* current = *cmds;
	cmd_t* content;
	int fds[2];
	while (current != NULL)
	{
		content = current->content;
		if (current->next != NULL)
		{
			if (pipe(fds) == -1)
				return 1;
			add_fd(content->output, fds[1]);
			add_fd(((cmd_t*)current->next->content)->input, fds[0]);
		}
		if (content->args[0] == NULL)
			;
		else if (content->executable == NULL)
			dprintf(2, "zzsh: command not found: %s\n", content->args[0]);
		else if (builtin_execute(content, data, fds[0], fds[1]) == 1)
		{
			if (execute(cmds, content, data->env))
			{
				cmd_close(content);
				return 1;
			}
			cmd_close(content);
		}
		current = current->next;
	}
	return 0;
}
