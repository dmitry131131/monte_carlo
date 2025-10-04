set(SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/Algorithm.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/Function.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/FunctionAST.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/FunctionTokenizer.cpp)

target_sources(${TARGET} PRIVATE ${SOURCES})
target_sources(${TEST_TARGET} PRIVATE ${SOURCES})