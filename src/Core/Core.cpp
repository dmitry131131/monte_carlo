#include "Core/Core.hpp"
#include "Core/Error.hpp"

namespace opts {
extern bool verbose;
} // opts

#define GENERAL_ERROR(MSG_) do {    \
    ERROR_MSG(MSG_);                \
    return -1;                      \
}while(0)

// TODO Система ошибок и уровней логирования 

Machine::Machine() : core_count_(cpuid_get_total_cpus()) {
    if (core_count_ <= 0) {
        throw machine_error("CpuId didn't get valid cpu count");
    }
}
namespace {
// Function get cpuid data from the system
int get_cpu_id_data(cpu_id_t& data) {
    if (!cpuid_present()) {
        GENERAL_ERROR("[ERROR] Sorry, your CPU doesn't support CPUID");
    }

    cpu_raw_data_t raw = {};

    if (cpuid_get_raw_data(&raw) < 0) {
        GENERAL_ERROR("[ERROR] Sorry, cannot get the CPUID raw data");
    }
    if (cpu_identify(&raw, &data) < 0) {
        GENERAL_ERROR("[ERROR] Sorry, CPU identification failed");
    }

    return 0;
}
} // namespace

void dump_machine_specs() noexcept {
    cpu_id_t data = {};

    if (get_cpu_id_data(data)) {
        ERROR_MSG("[ERROR] Sorry, machine data is not reachable");
        return;
    }
    
	MSG("Found: " << data.vendor_str << " CPU");                            // print out the vendor string (e.g. `GenuineIntel')
	MSG("Processor model is '" << data.cpu_codename << "'");                // print out the CPU code name (e.g. `Pentium 4 (Northwood)')
	MSG("The full brand string is `" << data.brand_str << "'");             // print out the CPU brand string
	MSG("The processor has " << data.l1_data_cache << "K L1 cache and " << data.l2_cache << "K L2 cache");                            // print out cache size information
	MSG("The processor has " << data.num_cores << " cores and " << data.num_logical_cpus << " logical processors");                        // print out CPU cores information

	MSG("Supported multimedia instruction sets:");
	MSG("  MMX         : " << (data.flags[CPU_FEATURE_MMX] ? "present" : "absent"));
	MSG("  MMX-extended: " << (data.flags[CPU_FEATURE_MMXEXT] ? "present" : "absent"));
	MSG("  SSE         : " << (data.flags[CPU_FEATURE_SSE] ? "present" : "absent"));
	MSG("  SSE2        : " << (data.flags[CPU_FEATURE_SSE2] ? "present" : "absent"));
	MSG("  3DNow!      : " << (data.flags[CPU_FEATURE_3DNOW] ? "present" : "absent"));

	MSG("CPU clock is: " << cpu_clock_by_os() << " MHz (according to your OS)");    // print out the CPU clock, according to the OS
	MSG("CPU clock is: " << cpu_clock_measure(200, 0) << " MHz (tested)");          // print out the CPU clock, measured with RDTSC
}

#undef GENERAL_ERROR