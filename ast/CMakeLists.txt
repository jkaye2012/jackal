set(ast_src_files
  "src/expression.cpp"
  "src/operator.cpp"
  "src/program.cpp"
  "src/node.cpp"
  "src/value.cpp"
  "src/visitor.cpp"
  )

add_library(jackal_ast STATIC ${ast_src_files})

add_subdirectory(tests)
