
set(ast_test_files
  "test_main.cpp"
)

add_executable(jackal_ast_tests ${ast_test_files})

target_link_libraries(jackal_ast_tests PRIVATE jackal_ast)
