#pragma once

int command_list_exec(cmd** cmds, lst** env)
int execute(cmd* input, lst** env);
int builtin_execute(cmd* input, lst** env);
int piper(cmd** cmds, lst** env);
char* get_executable_path(char* executable, lst** env);
