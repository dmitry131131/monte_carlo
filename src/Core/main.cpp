#include <iostream>
#include "CLI/CLI.hpp"

#include "Core/Error.hpp"
#include "Core/Core.hpp"
#include "Core/AppConfig.hpp"
#include "Algorithm/Algorithm.hpp"
#include "Dumper/Dumper.hpp"

#define GENERAL_ERROR(MSG_) do {    \
    ERROR_MSG(MSG_);                \
    return -1;                      \
}while(0)

// TODO добавить возможность указания интегрируемой функции 1 переменной

int main(int argc, char** argv) try {
    // Config app: scan command line, configure algorithm
    AppConfig app(argc, argv);
    if (app.parse_command_line()) {
        return 0;
    }

    // Create machine, scan system and count available cpu count
    Machine machine = app.machine_configure();

    Algorithm algorithm = app.algorithm_configure(machine);
    std::unique_ptr<Dumper> Dumper = app.dumper_configure();

    ERROR_MSG("0: " << algorithm.calculate_function(0));
    ERROR_MSG("1: " << algorithm.calculate_function(1));
    ERROR_MSG("2: " << algorithm.calculate_function(2));

    machine.get_monitor().start_monitoring();

    Algorithm::Result res = algorithm.launch();

    machine.get_monitor().stop_monitoring();

    // Launch algorithm and dumps result
    Dumper->dump(res);

    return 0;
}
catch (const std::runtime_error& err) {
    GENERAL_ERROR(err.what());
}
catch(...) {
    GENERAL_ERROR("Unknown error!");
}

#undef GENERAL_ERROR