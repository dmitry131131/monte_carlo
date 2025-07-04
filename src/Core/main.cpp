#include <iostream>
#include "CLI/CLI.hpp"

int main(int argc, char** argv) {
    CLI::App app("Monte-Carlo integration method benchmark");

    bool scan_machine{false};
    app.add_flag("-s,--scan-machine", scan_machine, "Scan machine core specs");

    CLI11_PARSE(app, argc, argv);

    if (scan_machine) {
        std::cout << "Scan" << std::endl;
    }
}