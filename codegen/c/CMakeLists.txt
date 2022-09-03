set(codegen_c_src_files
  "src/c_visitor.cpp"
  "src/dependency.cpp"
  "src/file_builder.cpp"
  )

add_library(jackal_codegen_c STATIC ${codegen_c_src_files})

target_link_libraries(jackal_codegen_c PRIVATE jackal_ast jackal_codegen)
