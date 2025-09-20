#include <fstream>
#include <filesystem>

#include "Monitor/Monitor.hpp"
#include "Core/Error.hpp"

Monitor::Monitor(bool enable) try : enabled_(enable) {
    if (!std::filesystem::exists(thermal_path)) {
        throw std::runtime_error{"Thermal path: \"" + thermal_path + "\" is not exist\n"};
    }

    for (const auto &path : std::filesystem::directory_iterator(thermal_path)) {
        if (!std::filesystem::exists(path.path().string() + "/type") || 
            !std::filesystem::exists(path.path().string() + "/temp")) {
            continue;
        }

        std::string type;
        std::ifstream(path.path().string() + "/type") >> type;

        thermal_zones_.emplace_back(ThermalZone{type, path.path().string()});
    }

    // FIXME set measuring period in config
    measuring_period_ = std::chrono::milliseconds(20);
}
catch (const std::runtime_error& err) {
    ERROR_MSG("[ERROR] can't create system monitor. Monitor disabled!");
    ERROR_MSG(err.what());

    enable = false;
}

void Monitor::get_temperature() {    
    for (auto& zone : thermal_zones_) {
        unsigned temp = 0;
        std::ifstream(zone.path_ + "/temp") >> temp;

        zone.create_record(static_cast<temp_t>(temp / temp_precision));
    }
}

void Monitor::ThermalZone::create_record(Monitor::temp_t temp) {
    temperature_.emplace_back(std::make_pair(chrono_t::now(), temp));
}

int Monitor::start_monitoring() try {
    if (!enabled_) {return 0;}

    is_monitoring_ = true;

    monitor_ = std::thread(&Monitor::monitoring, this);

    return 0;
}
catch (...) {
    ERROR_MSG("[ERROR] Fail in start monitoring! Monitor disabled!");
    enabled_ = false;
    is_monitoring_ = false;

    return 1;
}

int Monitor::stop_monitoring() {
    if (!enabled_) {return 0;}

    is_monitoring_ = false;

    monitor_.join();

    return 0;
}

int Monitor::monitoring() try {
    if (!enabled_) {return 0;}
    
    while (is_monitoring_)
    {
        get_temperature();
        std::this_thread::sleep_for(measuring_period_);
    }
    
    return 0;
}
catch(...) {
    ERROR_MSG("[ERROR] Fail during monitoring! Monitor disabled!");
    stop_monitoring();

    enabled_ = false;

    return 1;
}