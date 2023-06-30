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

int len(void** list)
{
  int index;
  for (index = 0; list[index]!=NULL; index++);
  return index;
}

int change_directory(char** args, lst** env)
{
  char cwd[PATH_MAX];
  char* oldpwd = get_env_variable(env, "PWD"); 
  if(len((void**)args)==1)
  {
    char* path = get_env_variable(env, "HOME");
    chdir(path);
    edit_env_variable(env, "OLDPWD", oldpwd); 
    if (getcwd(cwd, sizeof(cwd)) != NULL)
      {
        edit_env_variable(env, "PWD", cwd);
      }
    return 0;
  }
  if (chdir(args[1]) == 0)
  {
    edit_env_variable(env, "OLDPWD", oldpwd); 
    if (getcwd(cwd, sizeof(cwd)) != NULL)
      {
        edit_env_variable(env, "PWD", cwd);
      }
    return 0;  
  }
  else
  {
    dprintf(2, "Mauvais chemin : %s\n", args[1]);
    return 1;
  }
  
  return 0;
}

int builtin_execute(cmd* input, lst** env)
{
  if (strcmp(input->executable, "cd") == 0)
  {
    change_directory(input->args, env);
    return 0;
  }
  else
  {
    return 1;
  }
}

int execute(cmd* input, lst** env)
{
  int pid;
  int exitcode;
  if (input->executable != NULL)
    pid = fork();
  else
  {
    dprintf(2, "Executable inconnu\n");
    return 1;
  }
  if (pid == 0)
  {
    dup2(input->fd_out, 1);
    dup2(input->fd_in, 0);
    exitcode = execve(input->executable, input->args, get_env_str(env));
  }
  else
  {
    waitpid(pid, &exitcode, 0);
  }
  return exitcode;
}

int cmds_list_exec(lst** cmds, lst** env)
{
	lst* current = *cmds;
	cmd* content;
	int fds[2];
	while (current != NULL)
	{
		content = current->content;
		if (pipe(fds) < 0)
			return 1;
		if (current->next != NULL)
		{
			content->fd_out = fds[1];
			((cmd*)current->next->content)->fd_in = fds[0];
		}
		if (content->executable == NULL)
			dprintf(2, "zzsh: command not found: %s\n", content->args[0]);
		else if (builtin_execute(content, env) == 1)
			execute(content, env);
		current = current->next;
	}
	return 0;
}

char* get_executable_path(const char* executable, lst** env)
{
  int size_path_str;
  char * path_file;
  char* path_env = get_env_variable(env, "PATH");
  char** path_env_splited =split_quoted_charset(path_env, ":");
  
  for (int i=0; path_env_splited[i] != NULL; i++)
  {
    size_path_str = strlen(path_env_splited[i])+strlen(executable); 
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
