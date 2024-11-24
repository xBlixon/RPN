include(../config.cmake)

add_library(${LIBRARY_NAME} STATIC RPN.cpp)

target_include_directories(${LIBRARY_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
