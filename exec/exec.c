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
