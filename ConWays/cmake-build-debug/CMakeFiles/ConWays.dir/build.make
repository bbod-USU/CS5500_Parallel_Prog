# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/bradybodily/Repositories/CS5500_Parallel_Programming/ConWays

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/bradybodily/Repositories/CS5500_Parallel_Programming/ConWays/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/ConWays.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/ConWays.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ConWays.dir/flags.make

CMakeFiles/ConWays.dir/main.cpp.o: CMakeFiles/ConWays.dir/flags.make
CMakeFiles/ConWays.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/bradybodily/Repositories/CS5500_Parallel_Programming/ConWays/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/ConWays.dir/main.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ConWays.dir/main.cpp.o -c /Users/bradybodily/Repositories/CS5500_Parallel_Programming/ConWays/main.cpp

CMakeFiles/ConWays.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ConWays.dir/main.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/bradybodily/Repositories/CS5500_Parallel_Programming/ConWays/main.cpp > CMakeFiles/ConWays.dir/main.cpp.i

CMakeFiles/ConWays.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ConWays.dir/main.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/bradybodily/Repositories/CS5500_Parallel_Programming/ConWays/main.cpp -o CMakeFiles/ConWays.dir/main.cpp.s

# Object files for target ConWays
ConWays_OBJECTS = \
"CMakeFiles/ConWays.dir/main.cpp.o"

# External object files for target ConWays
ConWays_EXTERNAL_OBJECTS =

ConWays: CMakeFiles/ConWays.dir/main.cpp.o
ConWays: CMakeFiles/ConWays.dir/build.make
ConWays: CMakeFiles/ConWays.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/bradybodily/Repositories/CS5500_Parallel_Programming/ConWays/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ConWays"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ConWays.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ConWays.dir/build: ConWays

.PHONY : CMakeFiles/ConWays.dir/build

CMakeFiles/ConWays.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ConWays.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ConWays.dir/clean

CMakeFiles/ConWays.dir/depend:
	cd /Users/bradybodily/Repositories/CS5500_Parallel_Programming/ConWays/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/bradybodily/Repositories/CS5500_Parallel_Programming/ConWays /Users/bradybodily/Repositories/CS5500_Parallel_Programming/ConWays /Users/bradybodily/Repositories/CS5500_Parallel_Programming/ConWays/cmake-build-debug /Users/bradybodily/Repositories/CS5500_Parallel_Programming/ConWays/cmake-build-debug /Users/bradybodily/Repositories/CS5500_Parallel_Programming/ConWays/cmake-build-debug/CMakeFiles/ConWays.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/ConWays.dir/depend
