#ifndef CLIAPP_FILEREADER_H
#define CLIAPP_FILEREADER_H

#include <fstream>
#include <mutex>
#include <string>
#include <vector>
#include <filesystem>

class FileReader
{
public:
    explicit FileReader(const std::string& file_path);

    ~FileReader() = default;

    size_t readBytes(uint8_t* buffer, size_t size);

    std::vector<uint8_t> readBytes(size_t n_bytes);

    bool canRead();

private:
    std::ifstream m_file;
    std::filesystem::path m_path;
    std::once_flag m_flag;
};

#endif // CLIAPP_FILEREADER_H
