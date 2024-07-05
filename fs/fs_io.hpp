#pragma once

#include <fstream>
#include <limits>
#include <optional>

struct IOItem
{

    IOItem(const std::string& filename, bool in_binary_mode) : filename_(filename), binary(in_binary_mode)
    {
    }

    constexpr std::string get_target() { return filename_; }

protected:
    std::string filename_;
    bool binary = false;
    bool error_occurred = false;
};

struct IOWriter : public IOItem
{
    IOWriter(const std::string& filename, bool in_binary_mode) : IOItem(filename, in_binary_mode)
    {
        error_occurred = !CreateMissingDirectoriesFromPath(filename);
    }

    virtual bool IO_Write(const std::string& content) const;
    virtual bool IO_Append(const std::string& content) const;

private:
    bool CreateMissingDirectoriesFromPath(std::string path) const;
    void IO_WriteStream(std::ofstream& stream, const std::string& content) const {
        stream.write(content.data(), content.size());
    }
};

struct IOReader : public IOItem
{
    IOReader(const std::string& filename, bool in_binary_mode) : IOItem(filename, in_binary_mode) {}

    virtual std::optional<std::string> IO_Read(/*size_t num_bytes = std::numeric_limits<size_t>::max()*/) const;

private:
    std::string IO_ReadStream(std::ifstream& stream) const;

};

//these don't belong here but whatever
struct WaWIOWriter : public IOWriter
{
    WaWIOWriter(const std::string& relative_path, bool binary = false);
};
struct WaWIOReader : public IOReader
{
    WaWIOReader(const std::string& relative_path, bool binary = false);
};