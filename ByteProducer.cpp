#include "ByteProducer.h"
#include "FileReader.h"

class ByteProducer : public IByteProducer
{
public:
    ByteProducer() = delete;
    ByteProducer(const ByteProducer&) = delete;
    ByteProducer(ByteProducer&&) = delete;
    ByteProducer(const std::string& file_path, size_t chunk_size);

    ~ByteProducer() override = default;

    bool produce(std::vector<uint8_t>& destination) override;

private:
    size_t m_chunk_size;
    FileReader m_file_reader;
};

ByteProducer::ByteProducer(const std::string& file_path, size_t chunk_size)
  : m_file_reader{file_path}
  , m_chunk_size{chunk_size}
{
}

bool ByteProducer::produce(std::vector<uint8_t>& destination)
{
    destination = m_file_reader.readBytes(m_chunk_size);
    return m_file_reader.canRead();
}

IByteProducer* makeFileByteProducer(const std::string& file_path, size_t chunk_size)
{
    return static_cast<IByteProducer*>(new ByteProducer(file_path, chunk_size));
}