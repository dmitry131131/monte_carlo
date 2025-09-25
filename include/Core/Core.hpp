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

    std::optional<Monitor> monitor_;   ///< System monitor
public:
    Machine(Monitor::Settings settings, bool enable_monitor = false);
    Machine(const Machine& other)                   = delete;
    Machine& operator= (const Machine& other)       = delete;
    Machine(Machine&& other) noexcept               = default;
    Machine& operator= (Machine&& other) noexcept   = default;

    /// @brief get real logical count
    /// @return core count
    unsigned get_core_count() const {return core_count_;}
    bool monitor_enabled() const {
        if (!monitor_.has_value()) {
            return false;
        }
        return monitor_->enabled_;
    }
    /// @brief get Monitor 
    /// @return Monitor reference
    Monitor& get_monitor() {return monitor_.value();}
    /// @brief get const Monitor 
    /// @return const Monitor reference
    const Monitor& get_monitor() const {return monitor_.value();}
};

/// @brief Function dumps cpuid data in pretty view
void dump_machine_specs() noexcept;