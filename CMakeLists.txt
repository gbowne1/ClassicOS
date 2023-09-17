cmake_minimum_required(VERSION 3.13.4)
project(ClassicOS)

# Source files
set(BOOT_SOURCE_FILES
    src/boot/boot.asm
    src/boot/linker.ld
)
set(GRUB_SOURCE_FILES
    src/boot/grub/grub.cfg
    src/boot/grub/menu.lst
)
set(DRIVERS_SOURCE_FILES
    src/drivers/audio/audio.c
    src/drivers/audio/audio.h
    src/drivers/display/display.c
    src/drivers/display/display.h
    src/drivers/io/io.c
    src/drivers/io/io.h
    src/drivers/keyboard/keyboard.c
    src/drivers/keyboard/keyboard.h
    src/drivers/screen/screen.c
    src/drivers/screen/screen.h
    src/drivers/tty/tty.c
    src/drivers/tty/tty.h
)
set(KERNEL_SOURCE_FILES
    src/kernel/kernel.c
    src/kernel/kernel.h
    src/kernel/linker.ld
    src/kernel/print.c
)

add_executable(ClassicOS
    ${BOOT_SOURCE_FILES}
    ${GRUB_SOURCE_FILES}
    ${DRIVERS_SOURCE_FILES}
    ${KERNEL_SOURCE_FILES}
)

target_link_libraries(ClassicOS PRIVATE)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_LINKER ld)
set(CMAKE_EXE_LINKER_FLAGS "-g")
set(CMAKE_CXX_FLAGS "-g -Wall")
set(CMAKE_C_FLAGS "-g -Wall")
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
