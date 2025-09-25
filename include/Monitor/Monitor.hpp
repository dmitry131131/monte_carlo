/*!
    \file
    \brief File implements system monitor that scan and save system data during benchmark execution.
           For example core temperature

           Create separated thread which will record machine data during benchmark
           To implement CPU temperature scanning read /sys/class/thermal/thermal_zone0/temp
*/

#pragma once
#include <thread>
#include <vector>
#include <string>

/*!
	\brief Monitor class search temperature
*/
class Monitor final {
public:
    using temp_t         = double; 
    using chrono_t       = std::chrono::steady_clock;
    using time_point_t   = std::chrono::time_point<chrono_t>;
    using period_t       = std::chrono::duration<int, std::milli>;
    using temp_point_t   = std::pair<time_point_t, temp_t>;
    using temp_history_t = std::vector<temp_point_t>;

    /*!
        \brief Struct implements ThermalZone data
    */
    struct ThermalZone final {
        std::string type_;              ///< Type of thermal zone
        std::string path_;              ///< Path to Thermal zone
        time_point_t start_;            ///< Start time point
        temp_history_t temperature_;    ///< Temperature history of thermal zone

        ThermalZone(const std::string& type, const std::string& path) : type_(type), path_(path), start_(chrono_t::now()) {}
        /// \brief Add new record to temperature history
        void create_record(temp_t temp);
    };
    /*!
        \brief Struct implements Monitoring settings
    */
    struct Settings {
        period_t measuring_period_;   ///< Period of measuring 
        std::string thermal_path_;    ///< Path to thermal zones    
        unsigned temp_precision_;     ///< Precision of temperature measuring
    };
private:
    Settings settings_;             /// Settings of the Monitor
    bool is_monitoring_ = false;    /// Variable control monitoring cycle
    std::thread monitor_;           /// Monitoring thread 

    /// Thermal zone vector. Thermal zones enumerated and has /temp file with temperature 
    std::vector<ThermalZone> thermal_zones_;

    /// Realization of monitoring (run in separated monitoring thread)
    int monitoring();

    /// \brief Create measuring for all thermal zones
    void get_temperature();
public:
    bool enabled_ = false;

    Monitor(Settings settings, bool enable = false);
    Monitor(const Monitor& other)                   = delete;
    Monitor& operator= (const Monitor& other)       = delete;
    Monitor(Monitor&& other) noexcept               = default;
    Monitor& operator= (Monitor&& other) noexcept   = default;

    /// @brief get vector of ThermalZones
    const std::vector<ThermalZone> &get_thermal_zones() const {return thermal_zones_;}
    const Settings& get_settings() const {return settings_;}

    // Element access
    auto at(std::size_t pos) const {return thermal_zones_.at(pos);}
    auto operator[] (std::size_t pos) const {return thermal_zones_[pos];}
    auto front() const {return thermal_zones_.front();}
    auto back() const {return thermal_zones_.back();}
    const auto* data() const {return thermal_zones_.data();}

    // Iterators
    auto cbegin() const noexcept {return thermal_zones_.cbegin();}
    auto cend() const noexcept {return thermal_zones_.cend();}
    auto crbegin() const noexcept {return thermal_zones_.crbegin();}
    auto crend() const noexcept {return thermal_zones_.crend();}

    // Capacity
    bool empty() const noexcept {return thermal_zones_.empty();}
    std::size_t size() const noexcept {return thermal_zones_.size();}
    std::size_t max_size() const noexcept {return thermal_zones_.max_size();}
    std::size_t capacity() const noexcept {return thermal_zones_.capacity();}

    /*!
        \brief Start monitoring temperature (write temperature for temp_zones history)
               Launch monitor job in separated thread
    */
    int start_monitoring();
    /// @brief  stop monitoring and join monitoring thread
    /// @return 0
    int stop_monitoring();
};
