
set(test_files
  "test_main.cpp"
  "lexer/lexer_tests.cpp"
  "lexer/token_tests.cpp"
  )

add_executable(jackal_tests ${test_files})

target_include_directories(jackal_tests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(jackal_tests jackal_lexer)
