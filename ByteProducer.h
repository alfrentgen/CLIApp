#ifndef CLIAPP_BYTEPRODUCER_H
#define CLIAPP_BYTEPRODUCER_H

#include <cstddef>
#include <cinttypes>
#include <string>
#include <vector>
#include <memory>

class IByteProducer {
public:
    IByteProducer() = default;
    virtual ~IByteProducer() = default;

    virtual bool produce(std::vector<uint8_t>& destination) = 0;
};

IByteProducer* makeFileByteProducer(const std::string& file_path, size_t chunk_size = 4096);

#endif //CLIAPP_BYTEPRODUCER_H
