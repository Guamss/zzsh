#include "./builtin.h"

char* builtin_path(const char* executable)
{
  if (strcmp(executable, "cd") == 0 || strcmp(executable, "where") == 0 || strcmp(executable, "alias") == 0); 
  else
    return NULL;
  return strdup(executable);
}

int change_directory(char** args, lst** env)
{
  char cwd[PATH_MAX];
  char* oldpwd = get_env_variable(env, "PWD"); 
  if (tablen((const void**)args)==1)
  {
    char* path = get_env_variable(env, "HOME");
    chdir(path);
  }
  else if (tablen((const void**)args)>2)
  {
  dprintf(2, "cd : Trop d'arguments!\n");
    return 1;
  }
  else if (chdir(args[1]) != 0)
  {
    dprintf(2, "Mauvais chemin : %s\n", args[1]);
    return 1;
  }  
  edit_env_variable(env, "OLDPWD", oldpwd); 
  if (getcwd(cwd, sizeof(cwd)) != NULL)
    edit_env_variable(env, "PWD", cwd);
  return 0;
}

int where(char** args, lst** env)
{
  if (tablen((const void**)args)==1)
  {
    dprintf(2, "where : Il faut au moins 1 argument!\n");
    return 1;
  }
  char* exec;
  for (int i=1; args[i]!=NULL; i++)
  {
    exec = get_executable_path(args[i], env); 
    if (exec == NULL)
      printf("commande inconnue : %s\n", args[i]);
    else if (builtin_path(exec) != NULL)
      printf("%s : shell built-in command\n", exec);
    else  
      printf("%s\n", exec);
    free(exec);
  }
  return 0;
}

int builtin_execute(cmd_t* input, data_t *data, int fd_in, int fd_out)
{
  if (input->executable == NULL)
	  return 1;
  if (strcmp(input->executable, "cd") == 0)
  {
    change_directory(input->args, data->env);
    return 0;
  }
  else if(strcmp(input->executable, "where") == 0)
  {
    where(input->args, data->env);
    return 0;
  }
  else if (strcmp(input->executable, "alias") == 0)
	  return builtin_alias(fd_in, fd_out, data->aliases, input->args);
  else
  {
    return 1;
  }
}
