# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_COMMAND = /opt/CLion-2018.1.3/clion-2018.1.3/bin/cmake/bin/cmake

# The command to remove a file.
RM = /opt/CLion-2018.1.3/clion-2018.1.3/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/elias/Desktop/Sigmod2019

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/elias/Desktop/Sigmod2019/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/SortingAlgorithm.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/SortingAlgorithm.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/SortingAlgorithm.dir/flags.make

CMakeFiles/SortingAlgorithm.dir/src/psort.c.o: CMakeFiles/SortingAlgorithm.dir/flags.make
CMakeFiles/SortingAlgorithm.dir/src/psort.c.o: ../src/psort.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/elias/Desktop/Sigmod2019/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/SortingAlgorithm.dir/src/psort.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/SortingAlgorithm.dir/src/psort.c.o   -c /home/elias/Desktop/Sigmod2019/src/psort.c

CMakeFiles/SortingAlgorithm.dir/src/psort.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/SortingAlgorithm.dir/src/psort.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/elias/Desktop/Sigmod2019/src/psort.c > CMakeFiles/SortingAlgorithm.dir/src/psort.c.i

CMakeFiles/SortingAlgorithm.dir/src/psort.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/SortingAlgorithm.dir/src/psort.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/elias/Desktop/Sigmod2019/src/psort.c -o CMakeFiles/SortingAlgorithm.dir/src/psort.c.s

CMakeFiles/SortingAlgorithm.dir/src/psort.c.o.requires:

.PHONY : CMakeFiles/SortingAlgorithm.dir/src/psort.c.o.requires

CMakeFiles/SortingAlgorithm.dir/src/psort.c.o.provides: CMakeFiles/SortingAlgorithm.dir/src/psort.c.o.requires
	$(MAKE) -f CMakeFiles/SortingAlgorithm.dir/build.make CMakeFiles/SortingAlgorithm.dir/src/psort.c.o.provides.build
.PHONY : CMakeFiles/SortingAlgorithm.dir/src/psort.c.o.provides

CMakeFiles/SortingAlgorithm.dir/src/psort.c.o.provides.build: CMakeFiles/SortingAlgorithm.dir/src/psort.c.o


# Object files for target SortingAlgorithm
SortingAlgorithm_OBJECTS = \
"CMakeFiles/SortingAlgorithm.dir/src/psort.c.o"

# External object files for target SortingAlgorithm
SortingAlgorithm_EXTERNAL_OBJECTS =

SortingAlgorithm: CMakeFiles/SortingAlgorithm.dir/src/psort.c.o
SortingAlgorithm: CMakeFiles/SortingAlgorithm.dir/build.make
SortingAlgorithm: CMakeFiles/SortingAlgorithm.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/elias/Desktop/Sigmod2019/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable SortingAlgorithm"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/SortingAlgorithm.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/SortingAlgorithm.dir/build: SortingAlgorithm

.PHONY : CMakeFiles/SortingAlgorithm.dir/build

CMakeFiles/SortingAlgorithm.dir/requires: CMakeFiles/SortingAlgorithm.dir/src/psort.c.o.requires

.PHONY : CMakeFiles/SortingAlgorithm.dir/requires

CMakeFiles/SortingAlgorithm.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/SortingAlgorithm.dir/cmake_clean.cmake
.PHONY : CMakeFiles/SortingAlgorithm.dir/clean

CMakeFiles/SortingAlgorithm.dir/depend:
	cd /home/elias/Desktop/Sigmod2019/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/elias/Desktop/Sigmod2019 /home/elias/Desktop/Sigmod2019 /home/elias/Desktop/Sigmod2019/cmake-build-debug /home/elias/Desktop/Sigmod2019/cmake-build-debug /home/elias/Desktop/Sigmod2019/cmake-build-debug/CMakeFiles/SortingAlgorithm.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/SortingAlgorithm.dir/depend

