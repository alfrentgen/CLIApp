#ifndef CLIAPP_WORDCOUNTAPP_H
#define CLIAPP_WORDCOUNTAPP_H

#include <cinttypes>
#include <memory>
#include <string>
#include <vector>

#include "IApp.h"
#include "ByteProducer.h"

class CharReader;

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
    virtual uint64_t countWord();

private:
    const std::string m_word;
    std::unique_ptr<CharReader> m_char_reader;
};

#endif // CLIAPP_WORDCOUNTAPP_H
