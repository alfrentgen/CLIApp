#include <iostream>
#include <list>
#include <numeric>
#include <thread>

#include "CheckSumApp.h"
#include "ByteProducer.h"

using namespace std;

CheckSumApp::CheckSumApp(std::unique_ptr<IByteProducer>&& byte_producer, uint32_t max_queue_size)
  : m_byte_producer{std::move(byte_producer)}
  , m_result{0}
  , m_thread_pool{new ThreadPool(std::min(std::thread::hardware_concurrency(), max_queue_size))}
{
}

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

uint64_t CheckSumApp::calcCheckSum(const vector<uint8_t>& bytes)
{
    constexpr uint32_t byte_alignment = 4;
    auto counter = bytes.size() / byte_alignment;
    auto* ptr = reinterpret_cast<const uint32_t*>(bytes.data());
    auto result = accumulate(ptr, ptr + counter, 0);
    result += std::accumulate(bytes.begin() + counter * byte_alignment, bytes.end(), 0, [](uint32_t dst, const uint8_t byte) {
        dst <<= 8;
        dst |= byte;
        return dst;
    });
    return result;
}

int CheckSumApp::run()
{
    auto update_result = [this](const std::vector<uint8_t>& data) { m_result += calcCheckSum(data); };
    for (vector<uint8_t> buffer; m_byte_producer->produce(buffer);) {
        m_thread_pool->run(update_result, std::move(buffer));
    }
    m_thread_pool.reset();
    cout << "hashsum is: " << m_result << endl;
    return 0;
}
