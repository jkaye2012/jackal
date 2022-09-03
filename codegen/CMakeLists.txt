set(codegen_src_files
  "src/code_generator.cpp"
  "src/executable.cpp"
  )

add_library(jackal_codegen STATIC ${codegen_src_files})

add_subdirectory(c)
