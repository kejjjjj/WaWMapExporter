#pragma once


struct cmd_function_s;

cmd_function_s* Cmd_FindCommand(const char* cmd);
void Cmd_AddCommand(const char* name, void(*func)());
