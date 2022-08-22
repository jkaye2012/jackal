cmake_minimum_required(VERSION 3.20)

project(jackal)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS_RELEASE "-Ofast")

add_subdirectory(third_party)

# TODO: improve these, target_include_directories?
include_directories(third_party/spdlog/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(lexer)
add_subdirectory(bytecode)
add_subdirectory(ast)
add_subdirectory(parser)
add_subdirectory(codegen)
add_subdirectory(tests)

add_executable(main main.cpp)

# target_link_libraries(main PRIVATE spdlog::spdlog jackal_lexer jackal_bytecode jackal_ast jackal_parser)
