#include <map>
#include <stdexcept>

#include "ArgParser.h"

using namespace std;
using namespace argument_parser;

const ArgValue &IArgParser::operator[](const ArgKey &key) {
    static const optional<string> nothing = nullopt;
    return nothing;
};

class CArgParser : public IArgParser {
public:
    void addArgument(const Argument& argument) override;

    IArgParser &parse(int argc, const char **argv) override;

    const ArgValue &operator[](const ArgKey &key) override;

private:
    struct ArgDescriptor : Argument {
        bool parsed{};
        ArgValue value{};
    };

    map<ArgKey, ArgDescriptor> m_arguments;
};

ArgParserPtr IArgParser::makeParser(const initializer_list <Argument> &args) {
    auto parser_ptr = ArgParserPtr{new CArgParser()};
    for (auto &arg: args) {
        parser_ptr->addArgument(arg);
    }
    return parser_ptr;
}

void CArgParser::addArgument(const Argument& argument) {
    m_arguments[argument.key] = {argument};
}

IArgParser &CArgParser::parse(int argc, const char **argv) {
    int counter{};
    try {
        for (int i{}; i < argc; ++i, ++counter) {
            auto &arg_value = m_arguments.at(argv[i]);
            const auto next_i = i + 1;
            if (!arg_value.has_value) {
                arg_value.value = "";
            } else if (next_i == argc) {
                throw runtime_error(string("Argument ") + argv[i] + " has no value;");
            } else {
                arg_value.value = argv[next_i];
                ++i;
            }
            arg_value.parsed = true;
        }
    } catch (runtime_error &re) {
        throw re;
    } catch (...) {
        throw runtime_error("Unexpected argument.");
    }
    if (counter != m_arguments.size()) {
        throw runtime_error("Not enough arguments.");
    }
    return *this;
}

const ArgValue &CArgParser::operator[](const ArgKey &key) {
    if (m_arguments.count(key) && m_arguments.at(key).parsed) {
        return m_arguments[key].value;
    }
    return IArgParser::operator[]("");
}
