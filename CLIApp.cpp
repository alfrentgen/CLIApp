#include <array>
#include <filesystem>
#include <functional>
#include <iostream>
#include <map>
#include <tuple>


#include "IApp.h"
#include "ArgParser.h"
#include "FileReader.h"


using namespace std;
using namespace argument_parser;

class CApp : public IApp {
public:
    CApp() = default;
    ~CApp() override = default;
};

class HelpApp : public CApp {
public:
    HelpApp(const std::string& app_description, const std::vector<Argument>& args) {
        ostringstream is(app_description + '\n');
        for(auto& arg : args) {
            is << " " << arg.key << " " << arg.description << endl;
        }
        m_message = is.str();
    }

    int run() override {
        cout << m_message;
        return 0;
    }
private:
    std::string m_message;
};

class ReadFileApp : public CApp {
protected:
    ReadFileApp(const string& file_path) :
            m_file_reader{file_path} {}
    FileReader m_file_reader;
};

class CheckSumApp : public ReadFileApp {
public:
    CheckSumApp (const string& file_path) : ReadFileApp(file_path) {}

    int run() override {
        cout << "Calculating checksum." << endl;
        return 0;
    }
};

class CountWordApp : public ReadFileApp {
public:
    CountWordApp(const string& file_path, const string& word) : ReadFileApp(file_path)
    , m_word{word} {}

    int run() override {
        cout << "Counting words." << endl;
        return 0;
    }

    string m_word;
};

std::unique_ptr<IApp> makeApp(int argc, const char** argv) {
    const std::vector<Argument> options {
            Argument {"-h", false, "Prints help message."},
            Argument {"-f", true, "Specifies file to process. Usage: -f <filename>."},
            Argument {"-m", true, R"(Specifies mode. Allowed values: 'words' and 'checksum'. Usage: -m <mode>.)"},
            Argument {"-v", true, R"(Specifies a word to count. Used in the 'words' mode only. Usage: -m words -v <word>)"}};

    using AppMakerFunc = function<std::unique_ptr<IApp>()>;
    std::array<AppMakerFunc, 3> parser_apps{
            [&]() {
                auto parser = IArgParser::makeParser({options[0]});
                parser->parse(argc, argv);
                return std::unique_ptr<IApp>(new HelpApp("See the application usage below:\n", options));
            },
            [&]() {
                auto parser = IArgParser::makeParser({options[1], options[2]});
                parser->parse(argc, argv);
                const auto file_path = (*parser)["-f"];
                const auto mode = *(*parser)["-m"];
                if (!file_path || mode != "checksum") {
                    return std::unique_ptr<IApp>{};
                }
                return std::unique_ptr<IApp>(new CheckSumApp(*file_path));
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
                return std::unique_ptr<IApp>(new CountWordApp(*file_path, *word));
            }
    };

    std::unique_ptr<IApp> app;
    for (auto& app_maker : parser_apps) {
        try{
            if (app = app_maker())
                break;
            } catch (...) {
        }
    }

    if(!app) {
        app.reset(new HelpApp("Use -h for help", {}));
    }
    return app;
}