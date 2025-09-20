/*!
    \file
    \brief This code implement core classes and methods of Monte-Carlo integration method
*/
#pragma once
#include <stdexcept>
#include "libcpuid.h"
#include "Monitor/Monitor.hpp"

/*!
	\brief This class implements machine information data and system monitor
    This class collecting information about CPU temperature during algorithm
*/
class Machine final {
    int core_count_;    ///< count of available logical cpus

    Monitor monitor_;   ///< System monitor
public:
    Machine(bool enable_monitor = false);
    /// @brief get real logical count
    /// @return core count
    unsigned get_core_count() const {return core_count_;}
    /// @brief get Monitor 
    /// @return Monitor reference
    Monitor& get_monitor() {return monitor_;}
    /// @brief get const Monitor 
    /// @return const Monitor reference
    const Monitor& get_monitor() const {return monitor_;}
};

/// @brief Function dumps cpuid data in pretty view
void dump_machine_specs() noexcept;

class machine_error final : public std::runtime_error {
    using std::runtime_error::runtime_error;
};