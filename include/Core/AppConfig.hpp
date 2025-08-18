// File declare class encapsulate App class and collect all configuration info from command line
#pragma once
#include "CLI/CLI.hpp"

#include "Core/Core.hpp"
#include "Config/Config.hpp"
#include "Algorithm/Algorithm.hpp"
#include "Dumper/Dumper.hpp"

class AppConfig final {
    using limits_t = std::pair<double, double>;
    // Common App fields
    CLI::App app_ = CLI::App("Monte-Carlo integration method benchmark");
    const int argc_;
    const char* const * argv_;

    // Algorithm settings
    Algorithm::Settings settings_ = {DEFAULT_CORE_USAGE, DEFAULT_POINT_COUNT};
    // Function settings
    Function function_{[](double x){return x;}, 0, 0};

    // Dumper settings
    class DumpType {
    public:
        enum class DumpTypeVal : unsigned char {
            DEFAULT_CONSOLE,
            COLOR_CONSOLE,
            MARKDOWN
        } value_ = DumpTypeVal::DEFAULT_CONSOLE;
    private:
        std::unordered_map<std::string, DumpTypeVal> DumpTypeMap = {
            {"default-console", DumpTypeVal::DEFAULT_CONSOLE},
            {"color-console", DumpTypeVal::COLOR_CONSOLE},
            {"markdown", DumpTypeVal::MARKDOWN} 
        };
    public:
        DumpType() : value_(DumpTypeVal::DEFAULT_CONSOLE) {}
        DumpType(const std::string &str) {
            if (DumpTypeMap.find(str) != DumpTypeMap.end()) {
                value_ = DumpTypeMap.find(str)->second;
            }
        }
    } dumperType_;

    std::optional<std::string> outputFilename_ = std::nullopt;

public:
    AppConfig(const int argc, const char* const * argv);

    int parse_command_line();

    Algorithm configure(const Machine& machine);
    std::unique_ptr<Dumper> dumper_configure();
};