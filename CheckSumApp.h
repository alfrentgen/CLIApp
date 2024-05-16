#ifndef CLIAPP_CHECKSUMAPP_H
#define CLIAPP_CHECKSUMAPP_H

#include <atomic>
#include <memory>

#include "IApp.h"
#include "ByteProducer.h"
#include "FileReader.h"
#include "ThreadPool.h"

class CheckSumApp : public IApp
{
public:
    CheckSumApp() = delete;
    CheckSumApp(CheckSumApp&&) = delete;
    CheckSumApp(const CheckSumApp&) = delete;
    explicit CheckSumApp(std::unique_ptr<IByteProducer>&& byte_producer, uint32_t max_queue_size);
    ~CheckSumApp() = default;

    int run() override;

protected:
    virtual uint64_t calcCheckSum(const std::vector<uint8_t>& bytes);

private:
    std::unique_ptr<IByteProducer> m_byte_producer;
    std::atomic<uint64_t> m_result{0};

    std::unique_ptr<ThreadPool> m_thread_pool;
};

#endif // CLIAPP_CHECKSUMAPP_H
