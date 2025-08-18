// File implements different dumpers for Algorithm::Result
#include "Dumper/Dumper.hpp"

void OstreamDumper::dump(const Algorithm::Result &Result) {
    #define OS_MSG(MSG_) do{     \
        OS_ << MSG_<< std::endl; \
    }while(0)   

    if (!Result.is_valid()) {
        OS_MSG("Result is not valid!\n");
        return;
    }
    
    OS_MSG("Result: " << *Result.get_result());
    OS_MSG("Time: " << Result.get_duration()->count() << " sec");
    OS_MSG("CPU available: " << Result.get_machine().get_core_count());

    OS_MSG("\nAlgorithm settings:");
    OS_MSG(Result.get_algorithm_settings());

    OS_MSG("\nFunction info:");
    OS_MSG(Result.get_function());

    #undef OS_MSG
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
    OS_MSG("Start limit: " << Result.get_function().start_ << "\\");
    OS_MSG("End limit: " << Result.get_function().end_);
}