set(codegen_c_src_files
  "src/visitor.cpp"
  )

add_library(jackal_codegen_c STATIC ${codegen_c_src_files})
