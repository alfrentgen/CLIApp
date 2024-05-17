#include <map>
#include <stdexcept>

#include "ArgParser.h"

const ArgValue& IArgParser::operator[](const ArgKey& key)
{
    static const std::optional<std::string> nothing = std::nullopt;
    return nothing;
}

class CArgParser : public IArgParser
{
public:
    void addArgument(const Argument& argument) override;

    IArgParser& parse(int argc, const char** argv) override;

    const ArgValue& operator[](const ArgKey& key) override;

private:
    struct ArgDescriptor : Argument
    {
        bool parsed{};
        ArgValue value{};
    };

    std::map<ArgKey, ArgDescriptor> m_arguments;
};

ArgParserPtr makeParser(const std::initializer_list<Argument>& args)
{
    ArgParserPtr parser_ptr{new CArgParser()};
    for (auto& arg : args) {
        parser_ptr->addArgument(arg);
    }
    return parser_ptr;
}

void CArgParser::addArgument(const Argument& argument)
{
    m_arguments[argument.key] = {argument};
}

IArgParser& CArgParser::parse(int argc, const char** argv)
{
    int counter{};
    try {
        for (int i{}; i < argc; ++i, ++counter) {
            auto& arg_value = m_arguments.at(argv[i]);
            const auto next_i = i + 1;
            if (!arg_value.has_value) {
                arg_value.value = "";
            }
            else if (next_i == argc) {
                throw std::runtime_error(std::string("Argument ") + argv[i] + " has no value;");
            }
            else {
                arg_value.value = argv[next_i];
                ++i;
            }
            arg_value.parsed = true;
        }
    }
    catch (std::runtime_error& re) {
        throw re;
    }
    catch (...) {
        throw std::runtime_error("Unexpected argument.");
    }
    if (counter != m_arguments.size()) {
        throw std::runtime_error("Not enough arguments.");
    }
    return *this;
}

const ArgValue& CArgParser::operator[](const ArgKey& key)
{
    if (m_arguments.count(key) && m_arguments.at(key).parsed) {
        return m_arguments[key].value;
    }
    return IArgParser::operator[]("");
}
