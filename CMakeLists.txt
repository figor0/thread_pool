cmake_minimum_required(VERSION 3.5)

project(thread_pool LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(Private ${PROJECT_SOURCE_DIR}/src/thread_pool.cpp)
set(Public ${PROJECT_SOURCE_DIR}/include/thread_pool.h)

add_executable(thread_pool main.cpp ${Private} ${Public})

target_include_directories( thread_pool PUBLIC include )
