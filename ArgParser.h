#ifndef CLIAPP_ARGPARSER_H
#define CLIAPP_ARGPARSER_H

#include <initializer_list>
#include <string>
#include <tuple>
#include <memory>
#include <optional>
#include <vector>

namespace argument_parser {
    class IArgParser;

    using ArgParserPtr = std::unique_ptr<IArgParser>;

    using ArgKey = std::string;
    using ArgDescription = std::string;
    using ArgValue = std::optional<std::string>;

    constexpr auto unary_arg = false;

    struct Argument {
        ArgKey key;
        bool has_value;
        ArgDescription description;
    };

    class IArgParser {
    public:
        static ArgParserPtr makeParser(const std::initializer_list<Argument> &args = {});

        virtual void addArgument(const Argument &key) = 0;

        virtual IArgParser &parse(int argc, const char **argv) = 0;

        virtual const ArgValue &operator[](const ArgKey &key);
    };
}
#endif //CLIAPP_ARGPARSER_H
