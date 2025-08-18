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

// TODO написать продвинутую обработку исключений
// TODO добавить возможность указания интегрируемой функции 1 переменной
// TODO добавить слежение за состоянием машины в процессе работы алгоритма(отдельный процесс или поток)

int main(int argc, char** argv) {
    // Create machine, scan system and count available cpu count
    Machine machine;
    if (!machine.is_valid()) {
        GENERAL_ERROR("[ERROR] Unable to identify machine resources, to launch integration. Zero CPU available.");
    }
    // Config app: scan command line, configure algorithm
    AppConfig app(argc, argv);
    app.parse_command_line();

    Algorithm algorithm = app.configure(machine);
    std::unique_ptr<Dumper> Dumper = app.dumper_configure();

    // Launch algorithm and dumps result
    Dumper->dump(algorithm.launch());

    return 0;
}

#undef GENERAL_ERROR