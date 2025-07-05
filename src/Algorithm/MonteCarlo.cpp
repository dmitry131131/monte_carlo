#include "Algorithm/MonteCarlo.hpp"

double monte_carlo(Machine& machine, const std::function<double(double)>& f) {
    int core_count = machine.get_core_count();

    return f(2);
}