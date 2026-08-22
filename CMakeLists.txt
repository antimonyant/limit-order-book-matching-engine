cmake_minimum_required(VERSION 3.16)
project(limit_order_book_sim CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

add_compile_options(-Wall -Wextra)

add_executable(lob_sim src/main.cpp)
target_include_directories(lob_sim PRIVATE include)

# Tests are wired up on Day 3 once there's matching logic worth testing.
# enable_testing()
# add_subdirectory(tests)