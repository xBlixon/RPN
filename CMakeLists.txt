include(config.cmake)
cmake_minimum_required(VERSION 3.30)
project(RPN)
add_executable(${PROJECT_NAME} main.cpp)

add_subdirectory(lib)
target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBRARY_NAME})