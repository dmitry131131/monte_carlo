// File declare class encapsulate App class and collect all configuration info from command line
#pragma once
#include "CLI/CLI.hpp"

#include "Core/Core.hpp"
#include "Config/Config.hpp"
#include "Algorithm/Algorithm.hpp"

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
    
public:
    AppConfig(const int argc, const char* const * argv);

    int parse_command_line();

    Algorithm configure(const Machine& machine);
};