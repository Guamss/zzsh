#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../cmd/cmd.h"
#include <unistd.h>


int execute(cmd** input, char** env)
{
  int exitcode;
  for(int i=0; input[i]!=NULL; i++)
  {
    // création d'un processus enfant du programme
    const int pid = fork();
    // pid de l'enfant est 0
    if (pid == 0) 
    {
      // redirige la sortie standard vers un file descriptor
      dup2(1, input[i]->fd_out); 
      // on tue l'enfant (l'enfant devient la commande entrée par l'utilisateur)
      exitcode = execve(input[i]->executable, input[i]->args, env);
    }
  }
  return exitcode;
}

int len(char** list)
{
  int index;
  for (index = 0; list[index]!=NULL; index++);
  return index;
}

int change_directory(char** args)
{
  if(len(args)!=2)
  {
    printf("Mauvais arguments\n");
    return 1;
  }
  if(chdir(args[1])!=0)
  {
    printf("Mauvais chemin : %s\n", args[1]);
    return 1;
  }
  char cwd[PATH_MAX];
  if (getcwd(cwd, sizeof(cwd)) != NULL) 
  {
    printf("cd %s \nCurrent working dir: %s\n", args[1], cwd);
  }
  return 0;
}

int builtin_execute(cmd** input, char** env)
{
  int exitcode;
  (void) env;
  for(int i=0; input[i]!=NULL; i++)
  {
    if (strcmp(input[i]->executable, "cd") == 0)
    {
      exitcode = change_directory(input[i]->args);
      
    }
  }
  return exitcode;
}

int main()
{
  cmd** tests;
  tests = malloc(2*sizeof(cmd*));
  cmd test;
  test.executable = "cd";
  test.args = malloc(3*sizeof(char*));
  test.args[0] = "cd";
  test.args[1] = "..";
  test.args[2] = NULL;
  test.fd_in = 0;
  test.fd_out = 1;
  tests[0] = &test;
  tests[1] = NULL;
  builtin_execute(tests, NULL);
  return 0;
}
