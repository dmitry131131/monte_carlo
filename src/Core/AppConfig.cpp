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
    app_.add_option("-c,--core-count", settings_.core_usage_, "Set count of usable cores default value 1")->required();
    app_.add_option("-p,--point-count", settings_.point_count_, "Set count amount of integration points")->required();
    app_.add_option("-s,--start-limits", function_.start_, "Set start integration limits")->required();
    app_.add_option("-e,--end-limits", function_.end_, "Set end integration limits")->required();
}

int AppConfig::parse_command_line() {
    CLI11_PARSE(app_, argc_, argv_);

    if (settings_.point_count_ < settings_.core_usage_) {
        settings_.point_count_ = DEFAULT_POINT_COUNT;
        ERROR_MSG("[WARNING] Point count must be more then core usage, so set to " << settings_.point_count_ << " - default value\n");
    }

    if (function_.start_ >= function_.end_) {
        GENERAL_ERROR("[ERROR] first integration limit must be less then second limit");
    }

    VERBOSE_MSG(std::cout, "Function: \n" << function_ << "\nAlgorithm settings: \n" << settings_);

    return 0;
}

Algorithm AppConfig::configure(const Machine& machine) {
    return Algorithm{machine, function_, settings_};
}