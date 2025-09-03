// This code implement core classes and methods of Monte-Carlo integration method
#pragma once
#include <vector>
#include <chrono>
#include "libcpuid.h"


/// This class implements machine information data and system monitor
/// This class collecting information about CPU temperature during algorithm
class Machine {
    using temp_t = double; 
    using time_point_t = std::chrono::time_point<std::chrono::steady_clock>;
    using temp_history_t = std::vector<std::pair<time_point_t, temp_t>>;

    int core_count_;    /// count of available logical cpus
    bool valid_;        /// valid status of machine
    // TODO PID
    // TODO OS_TYPE_ENUM -> to use consistent temperature getter

    temp_history_t temperature_;
public:
    Machine();
    // Getters
    bool is_valid() const {return valid_;}
    unsigned get_core_count() const {return core_count_;}
    temp_history_t &get_temperature_history() {return temperature_;}

    // TODO template get_temperature function. Set instance for Linux and Windows 

    // TODO write temperature monitoring functions
    // Machine should start monitor process control it by IPC communication 

    // First method
    // Also should wait process ending after benchmark and copy temp vector to machine
    // In this function need to create shared memory to copy temperature vector into Machine
};

// Function dumps cpuid data in pretty view
void dump_machine_specs();
