#pragma once

#include <filesystem>
#include <string>
#include <vector>
#include <fstream>
#include <list>

namespace fs
{
	std::string exe_file_name();
	std::string exe_path();
	std::string get_extension(const std::string& path);
	std::string previous_directory(std::string& directory);
	std::string get_file_name(const std::string& fullpath);
	std::string get_file_name_no_extension(const std::string& fullpath);

	void create_file(const std::string& path);
	bool create_directory(const std::string& path);

	std::list<std::string> files_in_directory(const std::string& path);

	std::string get_last_error();

	inline bool directory_exists(const std::string& d) { return std::filesystem::exists(d); }
	inline bool file_exists(const std::string& f) { return std::filesystem::exists(f); }

	bool valid_file_name(const std::string& name);

}
