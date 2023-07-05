#include "./signal.h"

void	ctrlc(int num)
{
	printf("\n");
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	(void) num;
}

void	quit(int num)
{
	(void) num;
	// printf("Quit (core dumped)\n");
}

