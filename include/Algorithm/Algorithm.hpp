/*!
    \file
    \brief This file describe algorithm class with all runtime data
*/
#pragma once
#include <chrono>
#include <limits>
#include <optional>

#include "Core/Core.hpp"
#include "Algorithm/Function.hpp"

/*!
	\brief Class represents monte-carlo algorithm
*/
class Algorithm final {
public:
    /*!
        \brief Struct represents algorithm settings
    */
    struct Settings final {
        unsigned core_usage_;   ///< Core useage by the algorithm
        unsigned point_count_;  ///< Random generated points count usage by the algorithm

        friend std::ostream& operator<< (std::ostream& os, const Settings& settings);
    };

    /*!
        \brief Result of algorithm implementation.
        \details When Result object starts lifetime timer inside starts. 
                 After set_result() call inbuild timer stop 
    */
    class Result final {
    private:
        double result_; ///< Result of algorithm
        // Algorithm duration info 
        std::chrono::time_point<std::chrono::steady_clock> start_;  ///< Construction Result time point 
        std::chrono::time_point<std::chrono::steady_clock> end_;    ///< Finalization Result time pont
        std::chrono::duration<double> duration_;                    ///< Duration of Result object lifetime

        const Machine& machine_;                        ///< Machine information
        const Settings settings_;                       ///< Algorithm settings
        const Function::Settings function_settings_;    ///< Function integration information

        bool isValid_ = false;  ///< Result is not valid before the set_result() call
    public:
        /// Start object lifetime end start timer
        Result(const Machine& machine, const Settings& settings, Function::Settings function_settings);

        /// @brief Set result, stop timer and calculate duration, set Result valid
        void set_result(double result);

        bool is_valid() const {return isValid_;}
        std::optional<double> get_result() const;
        std::optional<std::chrono::duration<double>> get_duration() const;

        const Machine& get_machine() const {return machine_;}
        const Settings& get_algorithm_settings() const {return settings_;}
        const Function::Settings& get_function_settings() const {return function_settings_;}
    };

private:
    const Machine& machine_;    ///< Machine reference 
    const Settings settings_;   ///< Algorithm settings
    const Function function_;   ///< Target function 

public:
    Algorithm(const Machine& machine, const Function& func, const Settings& settings);
    /// @brief get machine reference
    /// @return const machine reference
    const Machine& get_machine() const {return machine_;}

    double calculate_function(double x) const {
        return function_(x);
    }

    /// @brief Launch Monte-Carlo algorithm
    Result launch();
};