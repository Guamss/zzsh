#include <stdlib.h>
#include <stdio.h>
#include "../cmd/cmd.h"
#include <unistd.h>


int execute(cmd** input, char** env)
{
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
      execve(input[i]->executable, input[i]->args, env);
    }
  }
}

int builtin_execute(cmd** input, char** env)
{
  for(int i=0; input[i]!=NULL; i++)
  {
    switch(input[i]->executable)
    {
      case "cd":
        change_directory(input[i]->args);
    }
  }
}

int change_directory(char** args)
{
  if(len(args)!=2)
  {
    printf("Mauvais arguments");
    return 1;
  }
  if(chdir(path[1])!=0)
  {
    printf("Mauvais chemin : %s", path);
    return 1;
  }
  return 0;
}

int len(char** list)
{
  for (int index=0; index!=NULL; index++);
  return index;
}

int main()
{
  cmd** tests;
  tests = malloc(2*sizeof(cmd*));
  cmd test;
  test.executable = "/bin/cat";
  test.args = malloc(3*sizeof(char*));
  test.args[0] = "cat";
  test.args[1] = "file";
  test.args[2] = NULL;
  test.fd_in = 0;
  test.fd_out = 1;
  tests[0] = &test;
  tests[1] = NULL;
  execute(tests, NULL);
  return 0;
}
