#include <array>
#include <filesystem>
#include <iostream>
#include <string>

#include "IApp.h"
#include "ArgParser.h"
#include "ByteProducer.h"
#include "CheckSumApp.h"
#include "WordCountApp.h"

using namespace std;
using namespace argument_parser;

class HelpApp : public IApp
{
public:
    HelpApp(const std::string& app_description, const std::vector<Argument>& args)
    {
        ostringstream is(app_description + '\n');
        for (auto& arg : args) {
            is << " " << arg.key << " " << arg.description << endl;
        }
        m_message = is.str();
    }

    int run() override
    {
        cout << m_message;
        return 0;
    }

    ~HelpApp() override = default;

private:
    std::string m_message;
};

std::unique_ptr<IApp> makeApp(int argc, const char** argv)
{
    const std::vector<Argument> options{Argument{"-h", false, "Prints help message."}, Argument{"-f", true, "Specifies file to process. Usage: -f <filename>."},
        Argument{"-m", true, R"(Specifies mode. Allowed values: 'words' and 'checksum'. Usage: -m <mode>.)"},
        Argument{"-v", true, R"(Specifies a word to count. Used in the 'words' mode only. Usage: -m words -v <word>)"}};

    using AppMakerFunc = function<std::unique_ptr<IApp>()>;
    std::array<AppMakerFunc, 3> parser_apps{[&] {
                                                auto parser = IArgParser::makeParser({options[0]});
                                                parser->parse(argc, argv);
                                                return std::unique_ptr<IApp>(new HelpApp("See the application usage below:\n", options));
                                            },
        [&] {
            auto parser = IArgParser::makeParser({options[1], options[2]});
            parser->parse(argc, argv);
            const auto file_path = (*parser)["-f"];
            const auto mode = *(*parser)["-m"];
            if (!file_path || mode != "checksum") {
                return std::unique_ptr<IApp>{};
            }
            return std::unique_ptr<IApp>(new CheckSumApp(
                std::unique_ptr<IByteProducer>(makeFileByteProducer(file_path.value(), sizeof(int32_t) * 1024)), std::thread::hardware_concurrency()));
        },
        [&]() {
            auto parser = IArgParser::makeParser({options[1], options[2], options[3]});
            parser->parse(argc, argv);
            const auto file_path = (*parser)["-f"];
            const auto mode = *(*parser)["-m"];
            const auto word = (*parser)["-v"];
            if (!file_path || mode != "words" || !word) {
                return std::unique_ptr<IApp>{};
            }
            return std::unique_ptr<IApp>(
                new WordCountApp(std::unique_ptr<IByteProducer>(makeFileByteProducer(file_path.value(), sizeof(int32_t) * 1024)), *word));
            return std::unique_ptr<IApp>{};
        }};

    std::unique_ptr<IApp> app;
    for (auto& app_maker : parser_apps) {
        try {
            if (app = app_maker(); app)
                break;
        }
        catch (...) {
        }
    }

    if (!app) {
        app.reset(new HelpApp("Use -h for help", {}));
    }
    return app;
}
