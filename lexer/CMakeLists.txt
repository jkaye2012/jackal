set(lexer_src_files
  "src/lexer.cpp"
  "src/token.cpp"
  )

add_library(jackal_lexer STATIC ${lexer_src_files})
