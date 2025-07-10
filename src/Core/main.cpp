#include <iostream>
#include "CLI/CLI.hpp"

#include "Core/Error.hpp"
#include "Core/Core.hpp"
#include "Algorithm/MonteCarlo.hpp"
#include "Algorithm/Algorithm.hpp"

bool verbose = false;

#define GENERAL_ERROR(MSG_) do {    \
    ERROR_MSG(MSG_);                \
    return -1;                      \
}while(0)

// TODO написать продвинутую обработку исключений
// TODO добавить возможность указания интегрируемой функции 1 переменной
// TODO добавить возможность выбора диапазона интегрирования

int main(int argc, char** argv) {
    Machine machine;
    if (!machine.is_valid()) {
        GENERAL_ERROR("[ERROR] Unable to identify machine resources, to launch integration. Zero CPU available.");
    }

    CLI::App app = CLI::App("Monte-Carlo integration method benchmark");

    int core_usage = DEFAULT_CORE_USAGE;
    long point_count = DEFAULT_POINT_COUNT;
    std::pair<double, double> integration_limits{0, 0};

    app.add_flag("-v,--verbose", verbose, "Enable verbose mode");
    app.add_flag_callback("--get-machine-specs", dump_machine_specs, "Dump all machine specs");
    app.add_option("-c,--core-count", core_usage, "Set count of usable cores default value 1")->required();
    app.add_option("-p,--point-count", point_count, "Set count amount of integration points")->required();
    app.add_option("-l,--integration-limits", integration_limits, "Set integration limits")->required();

    CLI11_PARSE(app, argc, argv);

    if (core_usage < 0) {
        core_usage = machine.get_core_count();
        if (verbose) {
            MSG("[WARNING] Core count must be more than 0, so set to " << core_usage << " - max available value");
        }
    }

    if (point_count < core_usage) {
        point_count = core_usage;
        if (verbose) {
            MSG("[WARNING] Point count must be more then core usage, so set to " << point_count << " - amount of usable cores");
        }
    }

    if (integration_limits.first >= integration_limits.second) {
        GENERAL_ERROR("[ERROR] first integration limit must be less then second limit");
    }

    Function func{[] (double x) {return x;}, integration_limits.first, integration_limits.second};
    Algorithm algorithm{machine, func, static_cast<unsigned>(core_usage), static_cast<unsigned>(point_count)};

    MSG(algorithm.launch());

    return 0;
}

#undef GENERAL_ERROR