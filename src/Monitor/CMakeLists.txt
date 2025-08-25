set(SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/Monitor.cpp)

target_sources(${TARGET} PRIVATE ${SOURCES})
target_sources(${TEST_TARGET} PRIVATE ${SOURCES})