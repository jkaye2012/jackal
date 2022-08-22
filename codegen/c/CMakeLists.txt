set(codegen_c_src_files
  "src/dependency.cpp"
  "src/file_builder.cpp"
  "src/visitor.cpp"
  )

add_library(jackal_codegen_c STATIC ${codegen_c_src_files})

target_link_libraries(jackal_codegen_c PRIVATE jackal_ast)
