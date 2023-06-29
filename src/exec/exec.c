#include <sys/wait.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../cmd/cmd.h"
#include <unistd.h>
#include "../env/env.h"

int execute(cmd* input, char** env)
{
  int exitcode;
  const int pid = fork();
  if (pid == 0)
  {
    dup2(input->fd_out, 1);
    dup2(input->fd_in, 0);
    exitcode = execve(input->executable, input->args, env);
  }
  else
  {
    waitpid(pid, &exitcode, 0);
  }
  return exitcode;
}

int len(void** list)
{
  int index;
  for (index = 0; list[index]!=NULL; index++);
  return index;
}

int piper(cmd** cmds, char** env)
{
  int fds[2];
  for (int i=0; cmds[i] != NULL; i++)
  {
    if (pipe(fds) < 0)
      return 1;
    if (i < len((void**)cmds)-1)
    {
    cmds[i]->fd_out = fds[1];
    cmds[i+1]->fd_in = fds[0];
    }
    execute(cmds[i], env);

  }
  return 0;
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
  int exitcode;
  if (strcmp(input->executable, "cd") == 0)
  {
    exitcode = change_directory(input->args, env);
  }
  return exitcode;
}
