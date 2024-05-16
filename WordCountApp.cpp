#include <utility>

#include "WordCountApp.h"

WordCountApp::WordCountApp(std::unique_ptr<IByteProducer>&& byte_producer, const std::string& word)
  : m_word{word}
  , m_byte_producer{std::move(byte_producer)}
{
}

uint64_t WordCountApp::countWords(const std::vector<uint8_t>& bytes)
{
    std::vector<uint8_t> buffer;
    m_byte_producer->produce(buffer);
    return {};
}

int WordCountApp::run()
{
    return 0;
}