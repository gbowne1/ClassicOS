# x86-baremetal-toolchain.cmake

# Specify the cross-compiler (If you need one; otherwise, your native GCC should work)
set(CMAKE_ASM_COMPILER nasm)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

# Add compiler and linker flags for x86 bare metal
set(CMAKE_ASM_NASM_FLAGS "-f elf32")
set(CMAKE_C_FLAGS "-m32 -ffreestanding -nostdlib")
set(CMAKE_CXX_FLAGS "-m32 -ffreestanding -nostdlib")
set(CMAKE_EXE_LINKER_FLAGS "-e kernel_main -T${CMAKE_CURRENT_LIST_DIR}/linker.ld -m32 -z noexecstack")