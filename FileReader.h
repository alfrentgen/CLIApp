#ifndef CLIAPP_FILEREADER_H
#define CLIAPP_FILEREADER_H

#include <fstream>
#include <string>
#include <tuple>

class FileReader {
public:
    explicit FileReader(const std::string& file_path);
    ~FileReader() = default;

    std::tuple<bool, size_t> reaBytes(uint8_t* buffer, size_t size) {
        bool good{m_file.read(reinterpret_cast<char*>(buffer), size).good()};
        return {good, m_file.gcount()};
    }
    std::ifstream m_file;
};


#endif //CLIAPP_FILEREADER_H
