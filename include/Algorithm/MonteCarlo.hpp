// In this file implementation of Monte Carlo integration algorithm
#pragma once

#include <functional>
#include "Core/Core.hpp"

double monte_carlo(Machine& machine, const std::function<double(double)>& f);

