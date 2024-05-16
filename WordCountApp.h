#ifndef CLIAPP_WORDCOUNTAPP_H
#define CLIAPP_WORDCOUNTAPP_H

#include <cinttypes>
#include <memory>
#include <string>
#include <vector>

#include "IApp.h"
#include "ByteProducer.h"

class WordCountApp : public IApp
{
public:
    WordCountApp() = delete;
    WordCountApp(WordCountApp&&) = delete;
    WordCountApp(const WordCountApp&) = delete;
    explicit WordCountApp(std::unique_ptr<IByteProducer>&& byte_producer, const std::string& word);
    ~WordCountApp() override = default;

    int run() override;

protected:
    virtual uint64_t countWords(const std::vector<uint8_t>& bytes);

private:
    std::unique_ptr<IByteProducer> m_byte_producer;
    const std::string m_word;
};

#endif //CLIAPP_WORDCOUNTAPP_H
