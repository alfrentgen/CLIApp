#ifndef CLIAPP_FILEREADER_H
#define CLIAPP_FILEREADER_H

#include <fstream>
#include <mutex>
#include <string>
#include <tuple>
#include <vector>

class FileReader
{
public:
    explicit FileReader(const std::string& file_path)
      : m_file{file_path} {};
    ~FileReader() = default;

    size_t readBytes(uint8_t* buffer, size_t size)
    {
        return m_file.read(reinterpret_cast<char*>(buffer), size).gcount();
    }

    std::vector<uint8_t> readBytes(size_t n_bytes)
    {
        std::vector<uint8_t> buffer(n_bytes, 0);
        buffer.resize(readBytes(buffer.data(), buffer.size()));
        return buffer;
    }

    bool canRead()
    {
        return m_file.good();
    }

private:
    std::ifstream m_file;
};

#endif // CLIAPP_FILEREADER_H
