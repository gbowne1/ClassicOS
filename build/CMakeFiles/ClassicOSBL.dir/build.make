# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/gbowne1/Documents/ClassicOS

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/gbowne1/Documents/ClassicOS/build

# Include any dependencies generated for this target.
include CMakeFiles/ClassicOSBL.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/ClassicOSBL.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ClassicOSBL.dir/flags.make

CMakeFiles/ClassicOSBL.dir/src/boot/boot.asm.o: CMakeFiles/ClassicOSBL.dir/flags.make
CMakeFiles/ClassicOSBL.dir/src/boot/boot.asm.o: ../src/boot/boot.asm
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/gbowne1/Documents/ClassicOS/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building ASM_NASM object CMakeFiles/ClassicOSBL.dir/src/boot/boot.asm.o"
	/usr/bin/nasm $(ASM_NASM_INCLUDES) $(ASM_NASM_FLAGS) -f elf -o CMakeFiles/ClassicOSBL.dir/src/boot/boot.asm.o /home/gbowne1/Documents/ClassicOS/src/boot/boot.asm

# Object files for target ClassicOSBL
ClassicOSBL_OBJECTS = \
"CMakeFiles/ClassicOSBL.dir/src/boot/boot.asm.o"

# External object files for target ClassicOSBL
ClassicOSBL_EXTERNAL_OBJECTS =

ClassicOSBL: CMakeFiles/ClassicOSBL.dir/src/boot/boot.asm.o
ClassicOSBL: CMakeFiles/ClassicOSBL.dir/build.make
ClassicOSBL: CMakeFiles/ClassicOSBL.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/gbowne1/Documents/ClassicOS/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking ASM_NASM executable ClassicOSBL"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ClassicOSBL.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ClassicOSBL.dir/build: ClassicOSBL

.PHONY : CMakeFiles/ClassicOSBL.dir/build

CMakeFiles/ClassicOSBL.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ClassicOSBL.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ClassicOSBL.dir/clean

CMakeFiles/ClassicOSBL.dir/depend:
	cd /home/gbowne1/Documents/ClassicOS/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/gbowne1/Documents/ClassicOS /home/gbowne1/Documents/ClassicOS /home/gbowne1/Documents/ClassicOS/build /home/gbowne1/Documents/ClassicOS/build /home/gbowne1/Documents/ClassicOS/build/CMakeFiles/ClassicOSBL.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/ClassicOSBL.dir/depend
