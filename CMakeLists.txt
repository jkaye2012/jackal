cmake_minimum_required(VERSION 3.20)

project(jackal)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS_RELEASE "-Ofast")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(bytecode)

add_executable(main main.cpp)

target_link_libraries(main jackal_bytecode)
