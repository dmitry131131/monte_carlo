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