// This file describe algorithm class with all runtime data
#pragma once
#include <chrono>
#include <limits>
#include <functional>

#include <Core/Core.hpp>

// TODO Think about class structure 
// For example what information needs in Result class.
// Maybe should inbuild Algorithm in Result class with Function struct 

/// Struct represents function in monte-carlo algorithm
struct Function {
    const std::function<double(double)> func_;
    double start_;
    double end_;

    Function(const std::function<double(double)>& func, double start, double end) :
                                                    func_(func), start_(start), end_(end) {}
    
    friend std::ostream& operator<< (std::ostream& os, const Function& func);
};

class Algorithm {
public:
    /// Struct represents algorithm settings
    struct Settings {
        unsigned core_usage_;
        unsigned point_count_;

        friend std::ostream& operator<< (std::ostream& os, const Settings& settings);
    };

    /// Result of algorithm implementation.
    /// When Result object starts lifetime timer inside starts. 
    /// After set_result() call inbuild timer stop 
    class Result {
    private:
        double result_;
        std::chrono::time_point<std::chrono::steady_clock> start_;
        std::chrono::time_point<std::chrono::steady_clock> end_;
        std::chrono::duration<double> duration_;

        const Machine& machine_;        // Machine information
        const Settings settings_;       // Algorithm settings
        const Function function_;       // Function integration information
    public:
        // Start object lifetime end start timer
        Result(const Machine& machine, const Settings& settings, const Function function);

        // Set result, stop timer and calculate duration
        void set_result(double result);

        // Getters
        double get_result() {return result_;}

        const Machine& get_machine() const {return machine_;}
        const Settings& get_algorithm_settings() const {return settings_;}

        // Dumper
        void dump_result();

        friend std::ostream& operator<< (std::ostream& os, const Result& result);
    };

private:
    const Machine& machine_;
    const Settings settings_;
    const Function function_;

public:
    Algorithm(const Machine& machine, const Function& func, const Settings& settings);

    const Machine& get_machine() const {return machine_;}

    Result launch();
};