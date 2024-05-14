#ifndef CLIAPP_CHECKSUMAPP_H
#define CLIAPP_CHECKSUMAPP_H

#include <atomic>
#include <condition_variable>
#include <deque>
#include <filesystem>
#include <functional>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

#include "IApp.h"
#include "ByteConsumer.h"
#include "ByteProducer.h"
#include "FileReader.h"
#include "ThreadPool.h"

class ByteProducer : public IByteProducer
{
public:
    ByteProducer() = delete;
    ByteProducer(const ByteProducer&) = delete;
    ByteProducer(ByteProducer&&) = delete;
    ByteProducer(const std::string& file_path, size_t chunk_size = 4096);

    ~ByteProducer() override = default;

    bool produce(std::vector<uint8_t>& destination) override;

private:
    size_t m_chunk_size;
    FileReader m_file_reader;
};

class CheckSumApp : public IApp
{
public:
    CheckSumApp() = delete;
    CheckSumApp(CheckSumApp&&) = delete;
    CheckSumApp(const CheckSumApp&) = delete;
    explicit CheckSumApp(std::unique_ptr<IByteProducer>&& byte_producer, uint32_t max_queue_size = std::thread::hardware_concurrency());
    ~CheckSumApp() = default;

    int run() override;

protected:
    virtual uint64_t calcCheckSum(const std::vector<uint8_t>& bytes);

private:
    std::unique_ptr<IByteProducer> m_byte_producer;

    const uint32_t max_queue_size;
    std::deque<std::vector<uint8_t>> m_queue;
    std::mutex m_queue_mtx;

    std::atomic<uint64_t> m_result{0};
    std::condition_variable m_cv;

    std::unique_ptr<ThreadPool> m_thread_pool;
};

#endif // CLIAPP_CHECKSUMAPP_H
