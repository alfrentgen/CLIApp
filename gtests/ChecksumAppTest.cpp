#include <algorithm>
#include <numeric>
#include <random>
#include <thread>

#include <gtest/gtest.h>
#include <ByteProducer.h>
#include <ThreadPool.h>

class RandomByteProducer : public IByteProducer
{
public:
    RandomByteProducer(size_t chunk_size = 4096)
      : m_chunk_size{chunk_size}
    {
    }

    ~RandomByteProducer() override = default;

    bool produce(std::vector<uint8_t>& destination) override
    {
        destination.resize(m_chunk_size, 0);
        std::generate(destination.begin(), destination.end(), std::function([this]() { return m_distrib(m_gen); }));
        return true;
    }

private:
    size_t m_chunk_size;
    std::mt19937 m_gen{17}; // mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<uint8_t> m_distrib;
};

TEST(ChecksumTest, multithreadTest)
{
    std::set<uint64_t> results;
    for (size_t i = 0, limit = 1023; i < limit; ++i) {
        RandomByteProducer rbp{};
        auto tp = std::make_unique<ThreadPool>(std::thread::hardware_concurrency());

        std::set<std::pair<size_t, uint64_t>> read_chunk_hashes;
        std::mutex mtx;
        std::set<std::pair<size_t, uint64_t>> processed_chunk_hashes;
        uint32_t chunk_counter{};
        std::atomic<uint32_t> call_counter{};

        std::atomic<uint64_t> result{};
        auto update_result = [&call_counter, &result, &mtx, &processed_chunk_hashes](size_t chunk_id, const std::vector<uint8_t>& data) {
            const auto chunk_hash = std::accumulate(data.cbegin(), data.cend(), uint64_t{0});
            result += chunk_hash;

            ++call_counter;
            auto lg = std::lock_guard(mtx);
            processed_chunk_hashes.emplace(chunk_id, chunk_hash);
        };

        uint64_t control_sum{};
        for (size_t j = 0; j < std::thread::hardware_concurrency() + 1; ++j) {
            std::vector<uint8_t> buffer;
            rbp.produce(buffer);
            const auto chunk_hash = std::accumulate(buffer.cbegin(), buffer.cend(), uint64_t{0});
            read_chunk_hashes.emplace(chunk_counter, chunk_hash);
            control_sum += chunk_hash;
            tp->run(update_result, chunk_counter, std::move(buffer));
            ++chunk_counter;
        }

        tp.reset();

        EXPECT_EQ(chunk_counter, call_counter) << "Failed at iteration #: " << i << std::endl
                                               << [&processed_chunk_hashes, &read_chunk_hashes] {
                                                      std::stringstream ss;
                                                      std::vector<std::pair<size_t, uint64_t>> diff;
                                                      std::set_symmetric_difference(processed_chunk_hashes.cbegin(), processed_chunk_hashes.cend(),
                                                          read_chunk_hashes.cbegin(), read_chunk_hashes.cend(), std::back_inserter(diff));
                                                      for (auto& [i, v] : diff) {
                                                          ss << i << ": " << v << ", ";
                                                      }
                                                      ss << std::endl;
                                                      return ss.str();
                                                  }();
        ASSERT_EQ(result, control_sum) << "Failed at iteration #: " << i << std::endl;
        results.emplace(result);
    }
}