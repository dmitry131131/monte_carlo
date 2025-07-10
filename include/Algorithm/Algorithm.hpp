// This file describe algorithm class with all runtime data
#pragma once
#include <chrono>
#include <limits>
#include <functional>

#include <Core/Core.hpp>

constexpr unsigned DEFAULT_POINT_COUNT = 1e5;
constexpr unsigned DEFAULT_CORE_USAGE = 1;
constexpr unsigned RANDOM_SEED = 1234;

// Struct represents function in monte-carlo algorithm
struct Function {
    const std::function<double(double)> func_;
    double start_;
    double end_;

    Function(const std::function<double(double)>& func, double start, double end);
    // TODO function dump
};

class Result {
private:
    double result_;
    std::chrono::time_point<std::chrono::steady_clock> start_;
    std::chrono::time_point<std::chrono::steady_clock> end_;
    std::chrono::duration<double> duration_;

    const Machine machine_;
    const unsigned core_usage_;
    const unsigned point_count_;
public:
    // Start object lifetime end start timer
    Result(const Machine& machine, const unsigned& core_usage, const unsigned& point_count);

    // Set result and stop timer
    void set_result(double result);
    double get_result() {return result_;}

    void dump_result();

    friend std::ostream& operator<< (std::ostream& os, const Result& result);
};

class Algorithm
{
private:
    // TODO Время старта и время окончания работы алгоритма
    unsigned core_usage_;
    unsigned point_count_; 
    const Machine machine_;
    Function function_;

public:
    Algorithm(Machine& machine, Function& func, unsigned core_usage);
    Algorithm(Machine& machine, Function& func, unsigned core_usage, unsigned point_count_);

    const Machine& get_machine() const {return machine_;}

    Result launch();
};