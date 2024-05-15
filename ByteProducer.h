#ifndef CLIAPP_BYTEPRODUCER_H
#define CLIAPP_BYTEPRODUCER_H

#include <cstddef>
#include <cinttypes>
#include <vector>

class IByteProducer {
public:
    IByteProducer() = default;
    virtual ~IByteProducer() = default;

    virtual bool produce(std::vector<uint8_t>& destination) = 0;
};

#endif //CLIAPP_BYTEPRODUCER_H
