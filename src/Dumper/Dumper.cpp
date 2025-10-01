// File implements different dumpers for Algorithm::Result
#include "Dumper/Dumper.hpp"

void DefaultDumper::dump(const Algorithm::Result &Result) {
    #define OS_MSG(MSG_) do{     \
        OS_ << MSG_<< std::endl; \
    }while(0)   

    if (!Result.is_valid()) {
        OS_MSG("Result is not valid!\n");
        return;
    }
    
    OS_MSG("Monte-Carlo results");
    OS_MSG("Result: " << *Result.get_result());
    OS_MSG("Time: " << Result.get_duration()->count() << " sec");
    OS_MSG("CPU available: " << Result.get_machine().get_core_count());

    OS_MSG("\nAlgorithm settings:");
    OS_MSG(Result.get_algorithm_settings());

    OS_MSG("\nFunction info:");
    OS_MSG(Result.get_function_settings());

    OS_MSG("\nTemperature info:");

    if (!Result.get_machine().monitor_enabled()) {
        OS_MSG("Monitor disabled!");
        return;
    }

    for (const auto& zone : Result.get_machine().get_monitor().get_thermal_zones()) {

        OS_MSG('\n' << zone.path_ << '\n' << zone.type_);
        for (const auto& temp : zone.temperature_) {
            OS_MSG(std::chrono::duration<double>(temp.first - zone.start_) << " : " << temp.second);
        }
    }

    #undef OS_MSG
}

void ColorDumper::dump(const Algorithm::Result &Result) {

    #define STYLE_OS_MSG(MSG_, STYLE_) do{  \
        set_style((STYLE_));                \
        OS_ << MSG_<< std::endl;            \
        set_style(StyleCode::RESET);        \
    }while(0)  
    
    #define OS_MSG(MSG_) do{     \
        OS_ << MSG_<< std::endl; \
    }while(0)   

    if (!Result.is_valid()) {
        STYLE_OS_MSG("Result is not valid!\n", StyleCode::RED);
        return;
    }
    
    set_style(StyleCode::BOLD);
    STYLE_OS_MSG("Monte-Carlo results", StyleCode::GREEN);
    OS_MSG("Result: " << *Result.get_result());
    OS_MSG("Time: " << Result.get_duration()->count() << " sec");
    OS_MSG("CPU available: " << Result.get_machine().get_core_count());

    STYLE_OS_MSG("\nAlgorithm settings:", StyleCode::BOLD);
    OS_MSG(Result.get_algorithm_settings());

    STYLE_OS_MSG("\nFunction info:", StyleCode::BOLD);
    OS_MSG(Result.get_function_settings());

    #undef OS_MSG
    #undef STYLE_OS_MSG
}

void MDDumper::dump(const Algorithm::Result &Result) {
    #define OS_MSG(MSG_) do{     \
        OS_ << MSG_<< std::endl; \
    }while(0)   

    if (!Result.is_valid()) {
        OS_MSG("#Result is not valid!\n");
        return;
    }

    OS_MSG("## Monte-Carlo results");
    OS_MSG("Result: " << *Result.get_result() << '\\');
    OS_MSG("Time: " << Result.get_duration()->count() << " sec\\");
    OS_MSG("CPU available: " << Result.get_machine().get_core_count());

    OS_MSG("\n## Algorithm settings:");
    OS_MSG("Core usage: " << Result.get_algorithm_settings().core_usage_ << "\\");
    OS_MSG("Point count: " << Result.get_algorithm_settings().point_count_);

    OS_MSG("\n## Function info:");
    OS_MSG("Start limit: " << Result.get_function_settings().start_ << "\\");
    OS_MSG("End limit: " << Result.get_function_settings().end_);
}