#pragma once

int execute(cmd* input, lst** env);
int builtin_execute(cmd* input, lst** env);
int piper(cmd** cmds, lst** env);
