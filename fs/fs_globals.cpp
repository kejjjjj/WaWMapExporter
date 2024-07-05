#include <Windows.h>
#include "fs_globals.hpp"
#include <direct.h>

namespace _fs = std::filesystem;


std::string fs::exe_file_name()
{
	char buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	return std::string(buffer);
}
std::string fs::exe_path()
{
	std::string f = exe_file_name();
	return f.substr(0, f.find_last_of("\\/"));
}
std::string fs::get_extension(const std::string& file)
{
	size_t const extensionPos = file.find_last_of(".");

	if (extensionPos == std::string::npos)
		return "";

	return file.substr(extensionPos);
}
std::string fs::previous_directory(std::string& directory)
{
	size_t pos = directory.find_last_of('\\');
	if (pos < 1 || pos == std::string::npos)
		return directory;

	return directory.substr(0, pos);
}
std::string fs::get_file_name(const std::string& fullpath)
{
	size_t pos = fullpath.find_last_of('\\');

	if (pos < 1 || pos == std::string::npos)
		return fullpath;

	return fullpath.substr(pos + 1);
}
std::string fs::get_file_name_no_extension(const std::string& fullpath)
{
	auto file = get_file_name(fullpath);
	auto extension = get_extension(file);
	return file.substr(0, file.size() - extension.size());

}

void fs::create_file(const std::string& path)
{
	std::fstream* nf = new std::fstream(path, std::ios_base::out);
	*nf << "";
	if (nf->is_open())
		nf->close();
	delete nf;
}
bool fs::create_directory(const std::string& path)
{
	return _mkdir((path).c_str()) != -1;
}
std::list<std::string> fs::files_in_directory(const std::string& path)
{
	std::list<std::string> files;

	if (!_fs::exists(path)) {
		return {};
	}

	for (const auto& entry : _fs::directory_iterator(path)) {
		if (entry.is_directory())
			continue;

		std::string str = entry.path().string();
		files.push_back(std::move(str));
	}

	return (files); //compiler I hope you optimize this! 
}
std::string fs::get_last_error()
{
	const DWORD errorMessageID = ::GetLastError();
	char* messageBuffer = nullptr;

	size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (char*)&messageBuffer, 0, NULL);

	std::string output = std::string(messageBuffer, size);

	LocalFree(messageBuffer);
	return output;
}
bool fs::valid_file_name(const std::string& name)
{
	if (name.empty() || name.length() >= MAX_PATH)
		return false;

	for (const auto& i : name) {
		if (!std::isalnum(i) && i != '-' && i != '_' && i != ' ')
			return false;

	}
	return true;
}