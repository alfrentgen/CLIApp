#include "FileReader.h"

FileReader::FileReader(const std::string& file_path) :
    m_file{file_path, std::ios::binary}
{}

class HashSumCalculator : public FileReader {
    ;
};