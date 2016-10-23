project(simple_crts CXX ASM)
cmake_minimum_required(VERSION 2.8.12)
set (CMAKE_CXX_STANDARD 14)

set_source_files_properties(jump_to_context_x86_64_elf_gas.S PROPERTIES COMPILE_FLAGS "-x assembler-with-cpp")

add_executable(simple_crts simple_crts.cpp jump_to_context_x86_64_elf_gas.S)
target_link_libraries(simple_crts pthread)
