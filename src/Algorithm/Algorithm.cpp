#include <cstdlib>
#include <thread>
#include <mutex>
#include <random>
#include <ctime>
#include <memory>

#include "Core/Error.hpp"
#include "Algorithm/Algorithm.hpp"

constexpr unsigned RANDOM_SEED = 1234;

namespace opts{
    extern bool verbose;
}

Algorithm::Algorithm(const Machine& machine, 
                     const Function& func, 
                     const Settings& settings) : machine_(machine),
                                                 settings_(settings),
                                                 function_(func) {}

namespace {
void integrate(const std::function<double(double)>& func, double start, 
               double end, unsigned count_points, double& result, std::mutex& mutex) {
    std::mt19937 engine(RANDOM_SEED);
    const double multi_const = (end - start) / static_cast<double>(engine.max());

    double local_result = 0;
    
    for (size_t _ = 0; _ < count_points; _++) {
        double point = start + static_cast<double>(engine())*multi_const;

        local_result += func(point);
    }

    std::lock_guard<std::mutex> lock(mutex);
    result += local_result;
}
}

Algorithm::Result Algorithm::launch() {
    std::vector<std::thread> thread_vector;
    double result = 0;
    std::mutex mutex;

    Result result_conclusion{machine_, settings_, function_};

    double step = (function_.get_end() - function_.get_start()) / static_cast<double>(settings_.core_usage_);
    double current_start = function_.get_start();
    unsigned points_per_frame = static_cast<unsigned>(settings_.point_count_ / settings_.core_usage_);

    for (size_t i = 0; i < settings_.core_usage_; i++) {
        thread_vector.emplace_back(integrate, function_.func_, current_start, current_start + step,
                                   points_per_frame, std::ref(result), std::ref(mutex));
        current_start += step;
    }

    for (auto& thread : thread_vector) {
        thread.join();
    }

    result_conclusion.set_result(result/static_cast<double>(settings_.point_count_));

    return result_conclusion; 
}

Algorithm::Result::Result(const Machine& machine, const Settings& settings, const Function function) :
               machine_(machine), settings_(settings), function_(function) {
    start_ = std::chrono::steady_clock::now();
}

void Algorithm::Result::set_result(double result) {
    end_ = std::chrono::steady_clock::now();
    duration_ = end_ - start_;
    result_ = result;

    isValid_ = true;
}

std::optional<double> Algorithm::Result::get_result() const {
    if (isValid_) {
        return result_;
    }

    return std::nullopt;
}

std::optional<std::chrono::duration<double>> Algorithm::Result::get_duration() const {
    if (isValid_) {
        return duration_;
    }

    return std::nullopt;
}

std::ostream& operator<< (std::ostream& os, const Algorithm::Settings& settings) {
    os << "Core usage: " << settings.core_usage_ << '\n';
    os << "Point count: " << settings.point_count_;

    return os;
}