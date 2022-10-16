
set(parser_test_files
  "test_main.cpp"
)

add_executable(jackal_parser_tests ${parser_test_files})

target_link_libraries(jackal_parser_tests PRIVATE jackal_parser)
