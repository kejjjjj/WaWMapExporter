#include "fs_io.hpp"
#include "fs_globals.hpp"

/***********************************************************************
 >                             IOWriter
***********************************************************************/
bool IOWriter::IO_Write(const std::string& content) const 
{

    if (error_occurred)
        return false;

    std::ofstream file(filename_, std::ios::out | (binary ? std::ios::binary : NULL));
    if (!file.is_open()) {
        return false;
    }

    IO_WriteStream(file, content);
    file.close();
    return true;
}
bool IOWriter::IO_Append(const std::string& content) const 
{
    if (error_occurred)
        return false;

    std::ofstream file(filename_, std::ios::app | (binary ? std::ios::binary : NULL));
    if (!file.is_open()) {
        return false;
    }
    IO_WriteStream(file, content);
    file.close();
    return true;
}
bool IOWriter::CreateMissingDirectoriesFromPath(std::string path) const
{
    size_t pos = 0;
    std::string progress;

    do {

        pos = path.find_first_of('\\');

        if (pos == std::string::npos)
            break;

        progress += path.substr(0, pos + 1);
        path = path.erase(0, pos + 1);

        progress.pop_back();
        if (!fs::directory_exists(progress)) {
            if (!fs::create_directory(progress)) {
                return false;
            }
        }
        progress += '\\';

    } while (true);

    return true;

}

/***********************************************************************
 >                             IOReader
***********************************************************************/
std::optional<std::string> IOReader::IO_Read(/*size_t num_bytes*/) const {

    if (error_occurred)
        return {};

    std::ifstream file(filename_, std::ios::in | (binary ? std::ios::binary : NULL));
    if (!file.is_open()) {
        return {};
    }

    auto content = IO_ReadStream(file);
    file.close();
    return content.length() ? std::make_optional(content) : std::nullopt;
}


std::string IOReader::IO_ReadStream(std::ifstream& stream) const {
    std::string buffer;
    try {
        char buf[4096];
        while (stream.read(buf, sizeof(buf))) {
            buffer.append(buf, size_t(stream.gcount()));
        }
        buffer.append(buf, size_t(stream.gcount()));

    }
    catch ([[maybe_unused]] std::ios_base::failure& ex) {
    }
    return buffer;
}


/***********************************************************************
 >                              AgentIO
***********************************************************************/

WaWIOWriter::WaWIOWriter(const std::string& relative_path, bool binary)
    : IOWriter(fs::exe_path() + '\\' + relative_path, binary) {}


WaWIOReader::WaWIOReader(const std::string& relative_path, bool binary)
    : IOReader(fs::exe_path() + '\\' + relative_path, binary) {}


