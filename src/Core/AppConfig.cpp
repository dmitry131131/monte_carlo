#include "Core/AppConfig.hpp"
#include "Core/Error.hpp"

namespace opts {
    bool verbose = false;
} // namespace opts

#define VERBOSE_MSG(STREAM, MSG_) do {  \
    if (opts::verbose) {                \
        STREAM << MSG_;                 \
    }                                   \
}while(0)

#define GENERAL_ERROR(MSG_) do {    \
    ERROR_MSG(MSG_);                \
    return -1;                      \
}while(0)

AppConfig::AppConfig(const int argc, const char* const* argv) : argc_(argc), argv_(argv) {
    app_.add_flag("-v,--verbose", opts::verbose, "Enable verbose mode");
    app_.add_flag_callback("--get-machine-specs", dump_machine_specs, "Dump all machine specs");
    app_.add_option("-c,--core-count", core_usage_, "Set count of usable cores default value 1")->required();
    app_.add_option("-p,--point-count", point_count_, "Set count amount of integration points")->required();
    app_.add_option("-l,--integration-limits", integration_limits_, "Set integration limits")->required();
}

int AppConfig::parse_command_line() {
    CLI11_PARSE(app_, argc_, argv_);

    if (core_usage_ < 0) {
        core_usage_ = DEFAULT_CORE_USAGE;
        VERBOSE_MSG(std::cout, "[WARNING] Core count must be more than 0, so set to " << core_usage_ << "default value\n");
    }

    if (point_count_ < core_usage_) {
        point_count_ = DEFAULT_POINT_COUNT;
        VERBOSE_MSG(std::cout, "[WARNING] Point count must be more then core usage, so set to " << point_count_ << " - default value\n");
    }

    if (integration_limits_.first >= integration_limits_.second) {
        GENERAL_ERROR("[ERROR] first integration limit must be less then second limit");
    }

    return 0;
}

Algorithm AppConfig::configure(Machine& machine) {
    Function func{[] (double x) {return x;}, integration_limits_.first, integration_limits_.second};

    return Algorithm{machine, func, static_cast<unsigned>(core_usage_), static_cast<unsigned>(point_count_)};
}