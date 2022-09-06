set(cli_src_files
  "src/driver.cpp"
  )

add_library(jackal_cli STATIC ${cli_src_files})

target_link_libraries(jackal_cli PRIVATE jackal_parser jackal_codegen_c)
