#include <iostream>
#include "CLI/CLI.hpp"

#include "Core/Error.hpp"
#include "Core/Core.hpp"
#include "Algorithm/MonteCarlo.hpp"

bool verbose = false;

#define GENERAL_ERROR(MSG_) do {    \
    ERROR_MSG(MSG_);                \
    return -1;                      \
}while(0)

int main(int argc, char** argv) {
    CLI::App app = CLI::App("Monte-Carlo integration method benchmark");

    app.add_flag("-v,--verbose", verbose, "Enable verbose mode");
    app.add_flag_callback("--get-machine-specs", dump_machine_specs, "Dump all machine specs");

    Machine machine;
    if (!machine.is_valid()) {
        GENERAL_ERROR("[ERROR] Unable to identify machine resources, to launch integration. Zero CPU available.");
    }

    CLI11_PARSE(app, argc, argv);

    MSG(monte_carlo(machine, [](double x) {return x*x;}));

    return 0;
}

#undef GENERAL_ERROR