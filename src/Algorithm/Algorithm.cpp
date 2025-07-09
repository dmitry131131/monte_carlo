#include <cstdlib>
#include <thread>
#include <mutex>
#include <random>
#include <ctime>
#include <memory>

#include "Core/Error.hpp"
#include "Algorithm/Algorithm.hpp"

extern bool verbose;

Algorithm::Algorithm(Machine& machine, 
                     Function& func, 
                     unsigned core_usage = 1) : core_usage_(core_usage),
                                          point_count_(DEFAULT_POINT_COUNT),
                                          result_(std::numeric_limits<double>::quiet_NaN()),
                                          machine_(machine),
                                          function_(func) {
    if (core_usage_ > machine_.get_core_count()) {
        core_usage_ = machine_.get_core_count();
        if (verbose) {
            MSG("[WARNING] Core count more then physical CPUS. Only " << core_usage_ << " cores will be used!");
        }
    }
}
Algorithm::Algorithm(Machine& machine, 
                     Function& func,
                     unsigned core_usage, 
                     unsigned point_count) : core_usage_(core_usage),
                                        point_count_(point_count),
                                        result_(std::numeric_limits<double>::quiet_NaN()),
                                        machine_(machine),
                                        function_(func) {}

Function::Function(const std::function<double(double)>& func, double start, double end) :
                              func_(func), start_(start), end_(end) {}

namespace {
void integrate(const std::function<double(double)>& func, double start, 
               double end, unsigned count_points, double& result, std::mutex& mutex) {
    std::mt19937 engine(RANDOM_SEED);
    const double multi_const = (end-start)/static_cast<double>(engine.max());

    double local_result = 0;
    
    for (size_t _ = 0; _ < count_points; _++) {
        double point = start + static_cast<double>(engine())*multi_const;

        local_result += func(point);
    }

    std::lock_guard<std::mutex> lock(mutex);
    result += local_result;
}
}

double Algorithm::launch() {
    std::vector<std::thread> thread_vector;
    double result = 0;
    std::mutex mutex;

    double step = (function_.end_ - function_.start_)/static_cast<double>(core_usage_);
    double current_start = function_.start_;
    unsigned points_per_frame = static_cast<unsigned>(point_count_ / core_usage_);

    for (size_t i = 0; i < core_usage_; i++) {
        thread_vector.emplace_back(integrate, function_.func_, current_start, current_start + step,
                                   points_per_frame, std::ref(result), std::ref(mutex));
        current_start += step;
    }

    for (auto& thread : thread_vector) {
        thread.join();
    }

    return result/static_cast<double>(point_count_);
}