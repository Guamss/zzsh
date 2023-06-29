#pragma once

int execute(cmd* input, char** env);
int builtin_execute(cmd* input, lst** env);
int piper(cmd** cmds, char** env);
