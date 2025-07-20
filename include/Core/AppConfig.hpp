// File declare class encapsulate App class and collect all configuration info from command line
#pragma once
#include "CLI/CLI.hpp"

#include "Core/Core.hpp"
#include "Config/Config.hpp"
#include "Algorithm/Algorithm.hpp"

class AppConfig final {
    using limits_t = std::pair<double, double>;

    CLI::App app_ = CLI::App("Monte-Carlo integration method benchmark");
    const int argc_;
    const char* const * argv_;

    int core_usage_ = DEFAULT_CORE_USAGE;
    long point_count_ = DEFAULT_POINT_COUNT;
    limits_t integration_limits_{0, 0};
    
public:
    AppConfig(const int argc, const char* const * argv);

    int parse_command_line();

    Algorithm configure(Machine& machine);

    // TODO Сделать класс контроля, инициализации и конфигурации машины, который при создании
    // конфигурирует машину, алгоритм.
};