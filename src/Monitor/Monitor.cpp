#include <fstream>
#include <filesystem>

#include "Monitor/Monitor.hpp"
#include "Core/Error.hpp"

Monitor::Monitor() {
    if (!std::filesystem::exists(thermal_path)) {
        ERROR_MSG("Thermal path: " << thermal_path << " is not exist\n");
        return;
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

int Monitor::start_monitoring() {
    is_monitoring = true;

    monitor_ = std::thread(&Monitor::monitoring, this);

    return 0;
}

int Monitor::stop_monitoring() {
    is_monitoring = false;

    monitor_.join();

    return 0;
}

int Monitor::monitoring() {
    while (is_monitoring)
    {
        get_temperature();
        std::this_thread::sleep_for(measuring_period_);
    }
    
    return 0;
}