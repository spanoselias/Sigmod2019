# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.14

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
CMAKE_COMMAND = /snap/clion/68/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /snap/clion/68/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/f/sigmod2019/theo

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/f/sigmod2019/theo/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/theo.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/theo.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/theo.dir/flags.make

CMakeFiles/theo.dir/main.c.o: CMakeFiles/theo.dir/flags.make
CMakeFiles/theo.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/f/sigmod2019/theo/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/theo.dir/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/theo.dir/main.c.o   -c /home/f/sigmod2019/theo/main.c

CMakeFiles/theo.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/theo.dir/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/f/sigmod2019/theo/main.c > CMakeFiles/theo.dir/main.c.i

CMakeFiles/theo.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/theo.dir/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/f/sigmod2019/theo/main.c -o CMakeFiles/theo.dir/main.c.s

# Object files for target theo
theo_OBJECTS = \
"CMakeFiles/theo.dir/main.c.o"

# External object files for target theo
theo_EXTERNAL_OBJECTS =

theo: CMakeFiles/theo.dir/main.c.o
theo: CMakeFiles/theo.dir/build.make
theo: CMakeFiles/theo.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/f/sigmod2019/theo/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable theo"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/theo.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/theo.dir/build: theo

.PHONY : CMakeFiles/theo.dir/build

CMakeFiles/theo.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/theo.dir/cmake_clean.cmake
.PHONY : CMakeFiles/theo.dir/clean

CMakeFiles/theo.dir/depend:
	cd /home/f/sigmod2019/theo/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/f/sigmod2019/theo /home/f/sigmod2019/theo /home/f/sigmod2019/theo/cmake-build-debug /home/f/sigmod2019/theo/cmake-build-debug /home/f/sigmod2019/theo/cmake-build-debug/CMakeFiles/theo.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/theo.dir/depend

