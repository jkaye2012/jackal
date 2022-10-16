set(parser_src_files
  "src/include.cpp"
  "src/keywords.cpp"
  "src/parse.cpp"
  "src/parse_error.cpp"
  )

add_library(jackal_parser STATIC ${parser_src_files})

target_link_libraries(jackal_parser PRIVATE jackal_ast jackal_lexer)

add_subdirectory(tests)
