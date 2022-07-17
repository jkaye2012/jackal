file(GLOB bytecode_src_files "src/*.cpp")

add_library(jackal_bytecode STATIC ${bytecode_src_files})
