# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.28.3/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.28.3/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/gumuhan/Documents/Waterloo/ECE650/code/m7gu/a4

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/gumuhan/Documents/Waterloo/ECE650/code/m7gu/a4/build

# Include any dependencies generated for this target.
include CMakeFiles/ece650-a4.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/ece650-a4.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/ece650-a4.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ece650-a4.dir/flags.make

CMakeFiles/ece650-a4.dir/ece650-a4.cpp.o: CMakeFiles/ece650-a4.dir/flags.make
CMakeFiles/ece650-a4.dir/ece650-a4.cpp.o: /Users/gumuhan/Documents/Waterloo/ECE650/code/m7gu/a4/ece650-a4.cpp
CMakeFiles/ece650-a4.dir/ece650-a4.cpp.o: CMakeFiles/ece650-a4.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/gumuhan/Documents/Waterloo/ECE650/code/m7gu/a4/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/ece650-a4.dir/ece650-a4.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ece650-a4.dir/ece650-a4.cpp.o -MF CMakeFiles/ece650-a4.dir/ece650-a4.cpp.o.d -o CMakeFiles/ece650-a4.dir/ece650-a4.cpp.o -c /Users/gumuhan/Documents/Waterloo/ECE650/code/m7gu/a4/ece650-a4.cpp

CMakeFiles/ece650-a4.dir/ece650-a4.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/ece650-a4.dir/ece650-a4.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/gumuhan/Documents/Waterloo/ECE650/code/m7gu/a4/ece650-a4.cpp > CMakeFiles/ece650-a4.dir/ece650-a4.cpp.i

CMakeFiles/ece650-a4.dir/ece650-a4.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/ece650-a4.dir/ece650-a4.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/gumuhan/Documents/Waterloo/ECE650/code/m7gu/a4/ece650-a4.cpp -o CMakeFiles/ece650-a4.dir/ece650-a4.cpp.s

# Object files for target ece650-a4
ece650__a4_OBJECTS = \
"CMakeFiles/ece650-a4.dir/ece650-a4.cpp.o"

# External object files for target ece650-a4
ece650__a4_EXTERNAL_OBJECTS =

ece650-a4: CMakeFiles/ece650-a4.dir/ece650-a4.cpp.o
ece650-a4: CMakeFiles/ece650-a4.dir/build.make
ece650-a4: minisat/libminisat.a
ece650-a4: /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX11.1.sdk/usr/lib/libz.tbd
ece650-a4: CMakeFiles/ece650-a4.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/gumuhan/Documents/Waterloo/ECE650/code/m7gu/a4/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ece650-a4"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ece650-a4.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ece650-a4.dir/build: ece650-a4
.PHONY : CMakeFiles/ece650-a4.dir/build

CMakeFiles/ece650-a4.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ece650-a4.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ece650-a4.dir/clean

CMakeFiles/ece650-a4.dir/depend:
	cd /Users/gumuhan/Documents/Waterloo/ECE650/code/m7gu/a4/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/gumuhan/Documents/Waterloo/ECE650/code/m7gu/a4 /Users/gumuhan/Documents/Waterloo/ECE650/code/m7gu/a4 /Users/gumuhan/Documents/Waterloo/ECE650/code/m7gu/a4/build /Users/gumuhan/Documents/Waterloo/ECE650/code/m7gu/a4/build /Users/gumuhan/Documents/Waterloo/ECE650/code/m7gu/a4/build/CMakeFiles/ece650-a4.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/ece650-a4.dir/depend

