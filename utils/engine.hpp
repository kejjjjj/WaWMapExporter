#pragma once
#include <type_traits>
#include <string>
#include <Windows.h>

template<typename T>
concept MemoryAddress_t = std::is_integral_v<T> || std::is_pointer_v<T>;

namespace Engine
{
	template<typename Return, MemoryAddress_t Mem, typename ... Args>
	constexpr inline Return call(const Mem address, Args... args)
	{
		return (reinterpret_cast<Return(*)(Args...)>(address))(args...);
	}
}

using namespace std::string_literals;

namespace Engine::Tools
{

	template<MemoryAddress_t Mem>
	void write_bytes(Mem dst, const std::string& bytes) {
		DWORD oldProtect = {};
		const auto size = bytes.length();
		VirtualProtect(reinterpret_cast<LPVOID>(dst), size, PAGE_EXECUTE_READWRITE, &oldProtect);
		memcpy_s(reinterpret_cast<LPVOID>(dst), size, bytes.c_str(), size);
		VirtualProtect(reinterpret_cast<LPVOID>(dst), size, oldProtect, &oldProtect);
	}

	template<MemoryAddress_t Mem>
	void nop(Mem dst) {
		write_bytes(dst, "\x90\x90\x90\x90\x90"s);
	}
	
}