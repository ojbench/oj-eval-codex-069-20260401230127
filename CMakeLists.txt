cmake_minimum_required(VERSION 3.10)
project(problem_069 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(code
    src/main.cpp
)

target_include_directories(code PRIVATE src)

if (MSVC)
  target_compile_options(code PRIVATE /W4)
else()
  target_compile_options(code PRIVATE -Wall -Wextra -Wpedantic)
endif()
