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

// TODO Add CLI groups with machine settings, algorithm settings and dumper settings
AppConfig::AppConfig(const int argc, const char* const* argv) : argc_(argc), argv_(argv) {
    app_.add_flag("-v,--verbose", opts::verbose, "Enable verbose mode");
    app_.add_flag_callback("--get-machine-specs", dump_machine_specs, "Dump all machine specs");
    app_.add_option("-c,--core-count", settings_.core_usage_, "Set count of usable cores default value 1")->default_val(DEFAULT_CORE_USAGE);
    app_.add_option("-p,--point-count", settings_.point_count_, "Set count amount of integration points")->default_val(DEFAULT_POINT_COUNT);
    app_.add_option("-s,--start-limits", function_.start_, "Set start integration limits")->default_val(0);
    app_.add_option("-e,--end-limits", function_.end_, "Set end integration limits")->default_val(1);

    app_.add_flag("--enable-monitor", monitorInfo_.monitor_enable_, "Enable the temperature monitor")->default_val(false);

    // dumperType_ has implicitly conversion from std::string
    app_.add_option("--output-format", dumperInfo_, "Set output format")->default_val("default-console")
        ->check(CLI::IsMember(dumperInfo_.DumpTypeMap));

    app_.add_option("-o,--output", dumperInfo_.outputFilename_, "Set output filename");
}

int AppConfig::parse_command_line() {
    try {
        app_.parse(argc_, argv_);
    } catch (const CLI::ParseError &e) {
        app_.exit(e);
        return 1;
    }

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

Machine AppConfig::machine_configure() {
    // TODO move this constants into .toml config
    Machine machine({std::chrono::milliseconds(20), "/sys/class/thermal/", 1000}, monitorInfo_.monitor_enable_);

    return machine;
}

Algorithm AppConfig::algorithm_configure(const Machine& machine) {
    return Algorithm{machine, function_, settings_};
}

std::unique_ptr<Dumper> AppConfig::dumper_configure() {
    #define ADD_DUMPER(DUMPER_) {                                       \
        if (!dumperInfo_.outputFilename_.has_value()) {                 \
            return std::make_unique<DUMPER_>(std::cout);                \
        }                                                               \
        return std::make_unique<DUMPER_>(*dumperInfo_.outputFilename_); \
    }

    switch (dumperInfo_.dumperType_)
    {
    case DumperInfo::DumpTypeVal::DEFAULT_CONSOLE:
        ADD_DUMPER(DefaultDumper);

    case DumperInfo::DumpTypeVal::COLOR_CONSOLE:
        ADD_DUMPER(ColorDumper)

    case DumperInfo::DumpTypeVal::MARKDOWN:
        ADD_DUMPER(MDDumper);

    default:
        throw std::runtime_error{"[ERROR] Unknown error in constructing Dumper. Invalid dumper type"};
    }

    #undef ADD_DUMPER
}
