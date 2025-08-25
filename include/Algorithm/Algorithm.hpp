// This file describe algorithm class with all runtime data
#pragma once
#include <chrono>
#include <limits>
#include <functional>
#include <optional>

#include <Core/Core.hpp>

/// Struct represents function in monte-carlo algorithm
struct Function final {
    const std::function<double(double)> func_;  /// Fuction f(x)
    double start_;                              /// Start integration limit
    double end_;                                /// End integration limit

    Function(const std::function<double(double)>& func, double start, double end) :
                                                    func_(func), start_(start), end_(end) {}
    
    friend std::ostream& operator<< (std::ostream& os, const Function& func);
};

class Algorithm final {
public:
    /// Struct represents algorithm settings
    struct Settings {
        unsigned core_usage_;   /// Core useage by the algorithm
        unsigned point_count_;  /// Random generated points count usage by the algorithm

        friend std::ostream& operator<< (std::ostream& os, const Settings& settings);
    };

    /// Result of algorithm implementation.
    /// When Result object starts lifetime timer inside starts. 
    /// After set_result() call inbuild timer stop 
    class Result final {
    private:
        double result_; // Result of algorithm
        // Algorithm duration info 
        std::chrono::time_point<std::chrono::steady_clock> start_;  
        std::chrono::time_point<std::chrono::steady_clock> end_;
        std::chrono::duration<double> duration_;

        const Machine& machine_;        // Machine information
        const Settings settings_;       // Algorithm settings
        const Function function_;       // Function integration information

        bool isValid_ = false;  // Result is not valid before the set_result() call
    public:
        // Start object lifetime end start timer
        Result(const Machine& machine, const Settings& settings, const Function function);

        // Set result, stop timer and calculate duration, set Result valid
        void set_result(double result);

        // Getters
        bool is_valid() const {return isValid_;}
        std::optional<double> get_result() const;
        std::optional<std::chrono::duration<double>> get_duration() const;

        const Machine& get_machine() const {return machine_;}
        const Settings& get_algorithm_settings() const {return settings_;}
        const Function& get_function() const {return function_;}
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