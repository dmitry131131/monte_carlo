/*!
    \file
    \brief File declare class encapsulate App class and collect all configuration info from command line
*/
#pragma once
#include "CLI/CLI.hpp"

#include "Core/Core.hpp"
#include "Algorithm/Algorithm.hpp"
#include "Dumper/Dumper.hpp"

/*!
	\brief Class-wrapper for CLI11 app with configuration functions
*/
class AppConfig final {
    using limits_t = std::pair<double, double>;
    static constexpr unsigned DEFAULT_POINT_COUNT = 1e8;
    static constexpr unsigned DEFAULT_CORE_USAGE = 2;
    // Common App fields
    CLI::App app_ = CLI::App("Monte-Carlo integration method benchmark");
    const int argc_;
    const char* const * argv_;

    // Algorithm settings
    Algorithm::Settings settings_ = {DEFAULT_CORE_USAGE, DEFAULT_POINT_COUNT};
    // Function settings
    Function function_{[](double x){return x;}, 0, 0};

    /// Dumper settings
    class DumpType final {
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
    } dumperType_; ///< Type of dumper

    std::optional<std::string> outputFilename_ = std::nullopt;  ///< Output filename 

public:
    AppConfig(const int argc, const char* const * argv);
    /// @brief function parse command line and fill config info 
    /// @return return 0 by success and -1 by fail
    int parse_command_line();
    
    /// @brief configure Algorithm using command line info 
    /// @param[in] machine Machine reference 
    /// @return Algorithm class ready for launch
    Algorithm configure(const Machine& machine);

    /// @brief configure Dumper using command line info
    /// @return unique_ptr on Dumper class
    std::unique_ptr<Dumper> dumper_configure();
};