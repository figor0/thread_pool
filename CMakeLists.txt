cmake_minimum_required(VERSION 3.5)

project(thread_pool LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(Private
    src/thread_pool.cpp
    src/atkin_sieve.cpp
)
set(Public
    include/thread_pool.h
    include/atkin_sieve.h
)

add_executable(thread_pool main.cpp ${Private} ${Public})

target_include_directories( thread_pool PUBLIC include )

target_link_libraries( thread_pool pthread)


