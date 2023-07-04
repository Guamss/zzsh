#include "../../lib/bozolib/bozolib.h"
#include "./alias.h"
#include <stdlib.h>
#include <stdio.h>
#include "../env/env.h"
#include <unistd.h>
#include <fcntl.h>
#include "../utils/utils.h"
#include <string.h>

lst** aliases_init(lst** env)
{
	lst** aliases;
  ssize_t file_name_size;
  char* home;
  int fd;
  char* file_path;
  ssize_t size_file_path;
  file_name_size = strlen("/.zzsh_aliases");
  home = get_env_variable(env, "HOME");
  size_file_path = strlen(home)+file_name_size;
  file_path = malloc((size_file_path+1)*sizeof(char));
  strcpy(file_path, home);
  strcat(file_path, "/.zzsh_aliases");
  fd = open(file_path, O_CREAT, 0644);
  if (fd == -1) 
    dprintf(2, "Erreur lors la création du fichier .zzsh_aliases\n");
  close(fd);
  fd = open(file_path, O_RDONLY);
  if (fd == -1)
    dprintf(2, "Erreur lors de la lecture du fichier .zzsh_aliases\n");
  int buff_size;
  buff_size = lseek(fd, 0, SEEK_END);
  close(fd);
  fd = open(file_path, O_RDONLY);
  char buffer[buff_size];
  read(fd, buffer, buff_size);
  char** assignation;
  char** line = split_quoted_charset(buffer, "\n");
 	aliases = malloc(sizeof(lst*));
	*aliases = NULL; 
  for(int i = 0; line[i]!=NULL; i++)
  {
    if (strchr(line[i], '=') != NULL)
    {
      assignation = split_quoted_charset(line[i], "=");
      add_alias(aliases, assignation[0], assignation[1]);
    }
  }
	return aliases;
}

int add_alias(lst** root, const char* key, const char* value)
{
	lst* nouveau;
	alias_t* alias;

	alias = malloc(sizeof(alias_t));
	if (alias == NULL)
		return 1;
	nouveau = malloc(sizeof(lst));
	if (nouveau == NULL)
	{
		free(alias);
		return 1;
	}
	alias->key = strdup(key);
	if (alias->key == NULL)
	{
		free(alias->key);
		free(alias);
		return 1;
	}
	alias->value = strdup(value);
	if (alias->key == NULL)
	{
		free(alias->key);
		free(alias->value);
		free(alias);
		return 1;
	}
	nouveau->content = alias;
	lst_addback(root, nouveau);
	return 0;
}

char* get_alias(lst** aliases, const char* key)
{
	lst* current = *aliases;
	alias_t* content;

	while (current != NULL)
	{
		content = current->content;
		if (strcmp(key, content->key) == 0)
			return content->value;
		current = current->next;
	}
	return NULL;
}

void alias_del(void *ptr)
{
	alias_t *alias = ptr;
	free(alias->key);
	free(alias->value);
	free(alias);
}
