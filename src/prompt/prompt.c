#include <stdio.h>
#include <limits.h>
#include <unistd.h>
#include "../../lib/bozolib/bozolib.h"
#include "../env/env.h"

char* get_prompt(lst** env)
{
  char* out;
  int size;
  char cwd[PATH_MAX];
  if (getcwd(cwd, sizeof(cwd)) == NULL)
  {
    dprintf(2, "chemin inconnu");
    return NULL;
  } 
  char* computer = get_env_variable(env, "COMPUTER");
  char* user = get_env_variable(env, "USER");
  char* home = get_env_variable(env, "HOME");
  if (computer == NULL)
    computer = "";
  if (user == NULL)
    user = "";
  else if (strncmp(home, cwd, strlen(home)) == 0)
  {
    strcpy(cwd, "~");
    strcat(cwd, cwd + strlen(home));
  }
  size = snprintf(NULL, 0, "%s@%s>%s $",user, computer, cwd);
  out = malloc(size*sizeof(char));
  if(out == NULL)
  {
    return NULL;
  }
  sprintf(out, "%s@%s > %s $",user, computer, cwd);
 
  return out;
}
