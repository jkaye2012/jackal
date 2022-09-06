
set(test_files
  "test_main.cpp"
  "codegen/c_codegen_tests.cpp"
  "lexer/lexer_tests.cpp"
  "lexer/token_tests.cpp"
  "parser/parse_tests.cpp"
  "util/exec_tests.cpp"
  "util/file_system_tests.cpp"
  "util/result_tests.cpp"
  )

add_executable(jackal_tests ${test_files})

target_include_directories(jackal_tests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(jackal_tests PRIVATE spdlog::spdlog jackal_lexer jackal_parser jackal_codegen_c)
