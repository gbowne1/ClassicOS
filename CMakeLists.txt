cmake_minimum_required(VERSION 4.0.0)
project(ClassicOS VERSION 0.0.1 LANGUAGES C ASM SH)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_ASM_COMPilER "nasm")
set(cmake_SH_COMPILER "bash")
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_FLAGS "-Wall -Wextra -pedantic -Werror")

# Define the executable name
set(EXECUTABLE_NAME "${PROJECT_NAME}")

# Set the repository URL
set(REPOSITORY_URL "https://github.com/gbowne1/ClassicOS/")

# Add the assembly and C source files
add_executable(${EXECUTABLE_NAME}
    boot.asm
    kernel.c
)

# Configure compiler flags for assembly
set(ASM_FLAGS "-f elf32")
set_property(SOURCE boot.asm PROPERTY COMPILE_OPTIONS "${ASM_FLAGS} -o boot.o")

# Configure compiler flags for C
set(C_FLAGS "-m32 -ffreestanding -nostdlib -fno-pie -std=c11")
set_target_properties(${EXECUTABLE_NAME} PROPERTIES COMPILE_FLAGS "${C_FLAGS}")

# Link the object files together
target_link_libraries(${EXECUTABLE_NAME} PRIVATE boot.o kernel.bin)

# Specify the output format and entry point
set_target_properties(${EXECUTABLE_NAME} PROPERTIES OUTPUT_FORMAT "binary")
set_target_properties(${EXECUTABLE_NAME} PROPERTIES ENTRYPOINT "boot")

# Include the linker script
include_directories(linker.ld)

# Add a custom message
message(STATUS "Building ${PROJECT_NAME} version ${PROJECT_VERSION}")
message(STATUS "Repository: ${REPOSITORY_URL}")
