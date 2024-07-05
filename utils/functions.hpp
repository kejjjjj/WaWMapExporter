#pragma once

#include <windows.h>
#include <string>
#include <format>
template<typename Return, typename ... Args>
inline Return engine_call(const bool cod4x, const unsigned long offset, Args... args)
{
	static HMODULE cod4x_handle = GetModuleHandle("cod4x_021.dll");

	if(!cod4x_handle)
		cod4x_handle = GetModuleHandle("cod4x_021.dll");

	return (reinterpret_cast<Return(*)(Args...)>(cod4x ? reinterpret_cast<unsigned long>(cod4x_handle) + offset : offset))(args...);
}
inline std::string get_current_date() {
	SYSTEMTIME st;
	GetLocalTime(&st);


	return std::format("{:04d}-{:02d}-{:02d} {:02d}:{:02d}:{:02d}", 
		st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
}

static const std::string base64_chars =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";

inline bool is_base64(unsigned char c) {
	return (isalnum(c) || (c == '+') || (c == '/'));
}

inline std::string base64_decode(const std::string& encoded_string) {
	int in_len = encoded_string.size();
	int i = 0;
	int j = 0;
	int in_ = 0;
	unsigned char char_array_4[4]{}, char_array_3[3]{};
	std::string ret;

	while (in_len-- && (encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
		char_array_4[i++] = encoded_string[in_]; in_++;
		if (i == 4) {
			for (i = 0; i < 4; i++)
				char_array_4[i] = static_cast<unsigned char>(base64_chars.find(char_array_4[i]));

			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0x0F) << 4) + ((char_array_4[2] & 0x3C) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x03) << 6) + char_array_4[3];

			for (i = 0; (i < 3); i++)
				ret += char_array_3[i];
			i = 0;
		}
	}

	if (i) {
		for (j = i; j < 4; j++)
			char_array_4[j] = 0;

		for (j = 0; j < 4; j++)
			char_array_4[j] = static_cast<unsigned char>(base64_chars.find(char_array_4[j]));

		char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
		char_array_3[1] = ((char_array_4[1] & 0x0F) << 4) + ((char_array_4[2] & 0x3C) >> 2);

		for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
	}

	return ret;
}
inline std::string base64_encode(const std::string& input) {
	std::string encoded_string;
	size_t len = input.size();
	size_t i = 0;

	while (i < len) {
		unsigned char char_array_3[3] = { 0 };
		unsigned char char_array_4[4] = { 0 };

		int j = 0;
		for (; j < 3 && (i + j) < len; ++j) {
			char_array_3[j] = input[i + j];
		}

		char_array_4[0] = (char_array_3[0] & 0xFC) >> 2;
		char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((j > 1) ? ((char_array_3[1] & 0xF0) >> 4) : 0);
		char_array_4[2] = ((j > 1) ? ((char_array_3[1] & 0x0F) << 2) + ((j > 2) ? ((char_array_3[2] & 0xC0) >> 6) : 0) : 0);
		char_array_4[3] = (j > 2) ? (char_array_3[2] & 0x3F) : 0;

		for (int k = 0; k < j + 1; ++k) {
			encoded_string += base64_chars[char_array_4[k]];
		}

		while (j++ < 3) {
			encoded_string += '=';
		}

		i += 3;
	}

	return encoded_string;
}


float random(const float range); //0 -> HI
float random(const float min, const float range); //LO -> HI