
set(lexer_test_files
  "test_main.cpp"
  "lexer_tests.cpp"
  "token_tests.cpp"
)

add_executable(jackal_lexer_tests ${lexer_test_files})

target_link_libraries(jackal_lexer_tests PRIVATE jackal_lexer)
