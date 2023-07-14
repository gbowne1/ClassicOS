cmake_minimum_required(VERSION 3.13.4)
project(ClassicOS)

add_library(lib STATIC
  src/lib/mylib.cpp
  src/lib/mylib.h
)

add_executable(ClassicOS
  src/boot/boot.asm
  src/kernel/kernel.c
  src/drivers/keyboard/keyboard.c
  src/drivers/screen/screen.c
)

target_link_libraries(ClassicOS PRIVATE
  lib
  pthread
)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_LINKER ld)
set(CMAKE_EXE_LINKER_FLAGS "-g")
set(CMAKE_CXX_FLAGS "-g")
set(CMAKE_C_FLAGS "-g")
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_ASM_COMPILER nasm)
set(CMAKE_SYSTEM_PROCESSOR i386)
set(CMAKE_ASM_NASM_COMPILER nasm)
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(CMAKE_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build)
set_target_properties(ClassicOS PROPERTIES LINK_FLAGS "-T ${CMAKE_CURRENT_SOURCE_DIR}/linker.ld")
