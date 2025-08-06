cmake_minimum_required(VERSION 3.28)
project(monte_carlo)

set(TARGET monte_carlo)
set(TEST_TARGET monte_carlo_test)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(${TARGET})
add_executable(${TEST_TARGET})

# Enable testing

enable_testing()

# Install dependencies
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/CPM.cmake) 

find_package(GTest REQUIRED)    # TODO Fix to CPM code

CPMAddPackage(
    NAME CLI11
    GIT_REPOSITORY "https://github.com/CLIUtils/CLI11"
    VERSION 2.5.0
)
CPMAddPackage(
    NAME cpuid
    GIT_REPOSITORY "https://github.com/anrieff/libcpuid"
    VERSION 0.8.0
)

# Link Libraries
target_link_libraries(${TARGET} PRIVATE CLI11::CLI11)
target_link_libraries(${TARGET} PRIVATE cpuid)

target_link_libraries(${TEST_TARGET} PRIVATE GTest::GTest GTest::Main)
target_link_libraries(${TEST_TARGET} PRIVATE cpuid)

# Include directory
target_include_directories(${TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_include_directories(${TEST_TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Compile options
target_compile_options(${TARGET} PUBLIC
    -Wall 
    -Wextra 
    $<$<CONFIG:DEBUG>:-g -fsanitize=address>
    $<$<CONFIG:RELEASE>:-O3>
)

# Link options
target_link_options(${TARGET} PRIVATE -lpthread
    $<$<CONFIG:DEBUG>: -fsanitize=address>
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test)

set_target_properties(${TARGET}
                      PROPERTIES
                      RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

install(TARGETS ${TARGET}
        RUNTIME DESTINATION bin)
