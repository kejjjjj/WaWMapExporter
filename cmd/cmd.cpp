#include "cg/cg_local.hpp"
#include "cg/cg_offsets.hpp"
#include "cmd.hpp"
#include <cstdint>
#include <iostream>
cmd_function_s cmds[24];
std::uint32_t cmd_iterator = {};
cmd_function_s* Cmd_FindCommand(const char* cmd)
{
	cmd_function_s* result = 0;
	const std::uint32_t f = 0x55CC70;
	__asm
	{
		mov esi, cmd;
		call f;
		mov result, eax;
	}

	return result;
}
void Cmd_AddCommand(const char* name, void(*func)())
{
	if (cmd_iterator >= 24 || Cmd_FindCommand(name)) {
		return;
	}
	
	auto& cmd = cmds[cmd_iterator++];

	cmd.function = func;
	cmd.name = name;
	cmd.next = *cmd_functions;
	*cmd_functions = &cmd;

}
