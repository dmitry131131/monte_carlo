// This code implement core classes and methods of Monte-Carlo integration method
#pragma once
#include "libcpuid.h"
#include "Monitor/Monitor.hpp"

// TODO cut temperature monitor to separated class 

/// This class implements machine information data and system monitor
/// This class collecting information about CPU temperature during algorithm
class Machine final {
    int core_count_;    /// count of available logical cpus
    bool valid_;        /// valid status of machine

    Monitor monitor_;
public:
    Machine();
    // Getters
    bool is_valid() const {return valid_;}
    unsigned get_core_count() const {return core_count_;}
    Monitor& get_monitor() {return monitor_;}
    const Monitor& get_monitor() const {return monitor_;}
};

// Function dumps cpuid data in pretty view
void dump_machine_specs();
