set(bytecode_src_files
  "src/instruction.cpp"
  "src/instruction_generator.cpp"
  "src/opcode.cpp"
  "src/operand.cpp"
  "src/reg.cpp"
  "src/vm.cpp"
  )

add_library(jackal_bytecode STATIC ${bytecode_src_files})
