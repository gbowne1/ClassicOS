cmake_minimum_required(VERSION 3.13.4)
project(ClassicOS VERSION 0.0.1 LANGUAGES C CXX ASM_NASM)

set(IS_OS_WINDOWS FALSE)
set(IS_OS_LINUX FALSE)
set(IS_OS_MAC FALSE)

if (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
	set(IS_OS_WINDOWS TRUE)
	message("Building for the Windows operating system.")
elseif (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Linux")
	set(IS_OS_LINUX TRUE)
	message("Building for the Linux operating system.")
elseif (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Darwin")
	set(IS_OS_MAC TRUE)
	message("Building for the Mac operating system.")
endif ()

# Source files
set(BOOT_SOURCE_FILES
    src/boot/boot.asm
    src/boot/linker.ld
)

#[[
set(GRUB_SOURCE_FILES
    src/boot/grub/grub.cfg
    src/boot/grub/menu.lst
)
]]

set(DRIVERS_SOURCE_FILES
    src/drivers/audio/audio.c
    src/drivers/audio/audio.h
	src/drivers/bus/eisa.c
	src/drivers/bus/eisa.h
	src/drivers/bus/isa.c
	src/drivers/bus/isa.h
	src/drivers/bus/mca.c
	src/drivers/bus/mca.h
	#src/drivers/bus/pci.asm
	src/drivers/bus/pci.c
	src/drivers/bus/pci.h
	src/drivers/bus/vesa.c
	src/drivers/bus/vesa.h
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
	src/kernel/arch/x86/include/memory.h
	src/kernel/arch/x86/include/types.h
	src/kernel/arch/x86/include/types.h
	src/kernel/arch/x86/memory/memory.c
	src/kernel/arch/x86/gdt.c
	src/kernel/arch/x86/gdt.c
	src/kernel/arch/x86/idt.c
	src/kernel/arch/x86/idt.h
	src/kernel/arch/x86/isr.c
	src/kernel/arch/x86/isr.h
    src/kernel/kernel.c
    src/kernel/kernel.h
    src/kernel/linker.ld
    src/kernel/print.c
)

set(UTIL_SOURCE_FILES
	src/EHS.h
	src/sys/cpu.h src/sys/cpu.cpp src/sys/CPU_GCC_AMD64.asm
	src/Util.h src/Util.cpp
	src/Version.h src/Version.cpp
	src/Serializer.h
	src/Array.h
	src/Vector.h
	src/SArray.h
	src/Str.h
	src/PRNG.h
	src/HRNG.h src/HRNG_GCC.s
	src/Math.h src/Math.cpp src/Math_GCC_AMD64.s
	src/Rect.h
	src/Range.h src/Range.cpp
	src/Color4.h src/Color4.cpp
	src/Color3.h src/Color3.cpp
	src/Quat.h
	src/Vec4.h
	src/Vec3.h
	src/Vec2.h
	src/Mat4.h
	src/Mat3.h
	src/Mat2.h

)

add_executable(ClassicOS
    ${GRUB_SOURCE_FILES}
    ${DRIVERS_SOURCE_FILES}
    ${KERNEL_SOURCE_FILES}
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Enable testing
enable_testing()

target_link_libraries(ClassicOS PRIVATE)


set(CMAKE_SYSTEM_PROCESSOR i386)
set(CMAKE_SYSTEM_NAME None)
set(CMAKE_C_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(CMAKE_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build)

if (IS_OS_LINUX)
	set(CMAKE_ASM_NASM_FLAGS "${CMAKE_ASM_NASM_FLAGS} -f elf")
	set(CMAKE_C_COMPILER gcc)
	set(CMAKE_LINKER ld)
	set(CMAKE_EXE_LINKER_FLAGS "-g -s")
	set(CMAKE_LINK_FLAGS "${CMAKE_LINK_FLAGS} -e kernel_main")
	set(CMAKE_CXX_FLAGS "-g -Wall")
	set(CMAKE_C_FLAGS "-g -Wall -m32")
	set(CMAKE_C_FLAGS_DEBUG "-g")
	set(CMAKE_GDB_COMMAND gdb)
	set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
	set_target_properties(ClassicOS PROPERTIES LINK_FLAGS "-T ${CMAKE_CURRENT_SOURCE_DIR}/linker.ld")
elseif (IS_OS_WINDOWS)
	set(CMAKE_ASM_NASM_FLAGS "${CMAKE_ASM_NASM_FLAGS} -f win32")
endif ()