// This code implement core classes and methods of Monte-Carlo integration method
#pragma once
#include <vector>

// This class implements core information
class Core
{
public:
    int id_;
    enum PerformanceType {
        LOW_POWER   = 0,
        PERFORMANCE = 1
    } type_;
    std::vector<int> temperature;
    
    Core(int id = 0, PerformanceType type = LOW_POWER) : id_(id), type_(type) {}

    // TODO написать dump информации в fstream
};

// This class implements machine information data
class Machine {

public:
    const int core_count_;
    Core* core_;

    // TODO написать парсер информации из системы для заполнения полей
    Machine();
    ~Machine();
};
