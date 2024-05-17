#include "FileReader.h"

FileReader::FileReader(const std::string& file_path)
  : m_file{file_path}
  , m_path{file_path} {};

size_t FileReader::readBytes(uint8_t* buffer, size_t size)
{
    std::call_once(m_flag, [&path = this->m_path] {
        if (std::filesystem::is_directory(path) || !std::filesystem::exists(path)) {
            throw std::runtime_error("Cannot read file: " + path.string());
        }
    });

    return m_file.read(reinterpret_cast<char*>(buffer), size).gcount();
}

std::vector<uint8_t> FileReader::readBytes(size_t n_bytes)
{
    std::vector<uint8_t> buffer(n_bytes, 0);
    buffer.resize(readBytes(buffer.data(), buffer.size()));
    return buffer;
}

bool FileReader::canRead()
{
    return m_file.good();
}