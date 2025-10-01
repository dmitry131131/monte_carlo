#pragma once
#include <functional>
#include <ostream>
#include <variant>

#include "Algorithm/FunctionAST.hpp"

/*!
	\brief Struct represents function in monte-carlo algorithm
*/
class Function final {
    NodePtr func_;                              ///< f(x)
    std::string text_;                          ///< Text variant of fuction
    double start_;                              ///< Start integration limit
    double end_;                                ///< End integration limit
public:
    Function(const std::string& text, double start, double end);

    double operator() (double x) {return func_->calculate(x);}

    // Getters
    double get_start() const {return start_;}
    double get_end() const {return end_;}
    
    friend std::ostream& operator<< (std::ostream& os, const Function& func);
};