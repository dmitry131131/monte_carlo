// This file describe algorithm class with all runtime data
#pragma once
#include <chrono>
#include <limits>
#include <functional>

#include <Core/Core.hpp>

constexpr unsigned DEFAULT_POINT_COUNT = 1e5;
constexpr unsigned DEFAULT_CORE_USAGE = 1;
constexpr unsigned RANDOM_SEED = 1234;


struct Function
{
    const std::function<double(double)> func_;
    double start_;
    double end_;

    Function(const std::function<double(double)>& func, double start, double end);
};

class Algorithm
{
private:
    // TODO Время старта и время окончания работы алгоритма
    unsigned core_usage_;
    unsigned point_count_; 
    double result_;
    const Machine& machine_;
    Function function_;

public:
    Algorithm(Machine& machine, Function& func, unsigned core_usage);
    Algorithm(Machine& machine, Function& func, unsigned core_usage, unsigned point_count_);

    double get_result() const {return result_;}
    const Machine& get_machine() const {return machine_;}

    double launch();
};