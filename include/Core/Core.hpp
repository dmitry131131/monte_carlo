// This code implement core classes and methods of Monte-Carlo integration method
#pragma once
#include <vector>
#include <chrono>
#include "libcpuid.h"


/// This class implements machine information data
class Machine {
    using temp_t = double; 
    using time_point_t = std::chrono::time_point<std::chrono::steady_clock>;
    using temp_history_t = std::vector<std::pair<time_point_t, temp_t>>;

    int core_count_;    /// count of available logical cpus
    bool valid_;        /// valid status of machine

    temp_history_t temperature_;
public:
    Machine();
    // Getters
    bool is_valid() const {return valid_;}
    unsigned get_core_count() const {return core_count_;}
    temp_history_t &get_temperature_history() {return temperature_;}

    // TODO write temperature monitoring functions
    // Machine should start monitor process control it by IPC communication 
    // Also should wait process ending after benchmark and copy temp vector to machine
};

// Function dumps cpuid data in pretty view
void dump_machine_specs();
