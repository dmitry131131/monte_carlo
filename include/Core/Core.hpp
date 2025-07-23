// This code implement core classes and methods of Monte-Carlo integration method
#pragma once
#include "libcpuid.h"

/// This class implements machine information data
class Machine {
    int core_count_;    /// count of available logical cpus
    bool valid_;        /// valid status of machine
public:
    Machine();

    bool is_valid() const {return valid_;}
    unsigned get_core_count() const {return core_count_;}
};

// Function dumps cpuid data in pretty view
void dump_machine_specs();
