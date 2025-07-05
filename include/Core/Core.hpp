// This code implement core classes and methods of Monte-Carlo integration method
#pragma once
#include <vector>
#include "libcpuid.h"

// This class implements core information
class Core
{
public:
    int id_;
    std::vector<int> temperature;
    
    explicit Core(int id = 0) : id_(id) {}

    // TODO написать dump информации в fstream
};

// This class implements machine information data
class Machine {
    int core_count_;
    std::vector<Core> core_;
    bool valid_;
public:
    Machine();

    bool is_valid() {return valid_;}
    int get_core_count() {return core_count_;}
};

int get_cpu_id_data(cpu_id_t& data);
void dump_machine_specs();
