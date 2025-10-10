/*!
    \file
    \brief This file describe Function used in Algorithm
*/
#pragma once
#include <functional>
#include <ostream>
#include <variant>

#include "Algorithm/FunctionAST.hpp"

/*!
	\brief Struct represents function in monte-carlo algorithm
*/
class Function final {
public:
    struct Settings final {
        std::string text_;  ///< Text variant of fuction
        double start_;      ///< Start integration limit
        double end_;        ///< End integration limit

        friend std::ostream& operator<< (std::ostream& os, const Settings& func);
    };

private:
    std::shared_ptr<FunctionNode> func_;    ///< function root node
    Settings settings_;                     ///< Function settings

    bool optimize();
public:
    Function(const std::string& text, double start, double end);

    double operator() (double x) const {return func_->calculate(x);}

    // Getters
    const Settings& get_settings() const {return settings_;}
    const  std::shared_ptr<FunctionNode> get_root() const {return func_;}
    const std::string& get_text() const {return settings_.text_;}
    double get_start() const {return settings_.start_;}
    double get_end() const {return settings_.end_;}
    
    friend std::ostream& operator<< (std::ostream& os, const Function& func);
};