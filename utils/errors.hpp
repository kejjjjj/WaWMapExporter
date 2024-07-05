#pragma once

#include <string>
#include <format>
#include <utility>

#include "global_macros.hpp"

#pragma warning(push)
#pragma warning (disable : 4996)

template<typename ... Args>
inline void FatalError(const std::string& format, const Args&&... args)
{
	char buffer[512];
	std::snprintf(buffer, sizeof(buffer), format.c_str(), args...);
	*(bool*)0xD5EC496 = true;
	strcpy((char*)0x1475ED0, buffer);
	((void(*)())0x4FD030)();

}
template<>
inline void FatalError(const std::string& format)
{


	*(bool*)0xD5EC496 = true;
	strcpy((char*)0x1475ED0, format.c_str());
	((void(*)())0x4FD030)();

}
#pragma warning(pop)
