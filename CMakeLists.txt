cmake_minimum_required(VERSION 3.13.4)

# Enable C, Assembly, and Shell scripting languages
enable_language(C ASM SH)

# Set up the project name and version
set(PROJECT_NAME "ClassicOS")
set(PROJECT_VERSION "0.0.1")

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
