#ifndef CLIAPP_BYTECONSUMER_H
#define CLIAPP_BYTECONSUMER_H

#include <cstddef>
#include <cinttypes>
#include <vector>

class IByteConsumer {
public:
    IByteConsumer() = default;
    virtual ~IByteConsumer() = default;

    virtual bool consume(std::vector<uint8_t>& source) = 0;
};


#endif //CLIAPP_BYTECONSUMER_H
