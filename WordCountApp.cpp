#include <iostream>
#include <list>
#include <map>
#include <stdexcept>
#include <utility>

#include "WordCountApp.h"

class CharReader
{
public:
    CharReader(std::unique_ptr<IByteProducer>&& byte_producer, size_t buffer_size)
      : m_byte_producer(std::move(byte_producer))
    {
        m_buffer.reserve(buffer_size);
    }

    bool getNextChar(char& ch)
    {
        if (m_buf_position == m_buffer.size()) {
            m_stop = !m_byte_producer->produce(m_buffer);
            m_buf_position = 0;
        }

        if (m_buf_position == m_buffer.size() && m_stop) {
            return false;
        }

        ch = m_buffer.at(m_buf_position++);
        return true;
    }

private:
    std::unique_ptr<IByteProducer> m_byte_producer;
    size_t m_buf_position{};
    std::vector<uint8_t> m_buffer;
    bool m_stop{};
};

WordCountApp::WordCountApp(std::unique_ptr<IByteProducer>&& byte_producer, const std::string& word)
  : m_word{word}
  , m_char_reader(new CharReader(std::unique_ptr<IByteProducer>{std::move(byte_producer)}, word.size()))
{
}

uint64_t WordCountApp::countWord()
{
    size_t stream_position{};
    uint64_t word_count{};
    using candidate_t = std::pair<size_t, size_t>;
    std::list<candidate_t> candidates;

    for (char ch{}; m_char_reader->getNextChar(ch); ++stream_position) {
        for (auto it = candidates.begin(); it != candidates.end();) {
            if (const auto [ref_pos, ref_len] = *it; ref_len == m_word.size()) { // trim the candidate list head
                const auto remove_predicate = [&](const candidate_t& cur) { return ref_pos <= cur.first && cur.first < ref_pos + ref_len < cur.first; };
                for (; it != candidates.end() && remove_predicate(*it);) {
                    candidates.erase(it++);
                }
                ++word_count;
                continue;
            }
            else { // check the rest
                if (m_word[ref_len] == ch) {
                    it->second += 1;
                }
                else {
                    candidates.erase(it++);
                }
            }
        }

        if (ch == m_word[0]) { // add new candidate
            candidates.emplace_back(stream_position, 1);
        }
    }
    return word_count;
}

int WordCountApp::run()
{
    std::cout << R"(The word ")" + m_word + R"(" occured )" << countWord() << " times.";
    return 0;
}