# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/yuna/cpp_server

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/yuna/cpp_server/build

# Include any dependencies generated for this target.
include CMakeFiles/math.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/math.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/math.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/math.dir/flags.make

CMakeFiles/math.dir/math/main.cpp.o: CMakeFiles/math.dir/flags.make
CMakeFiles/math.dir/math/main.cpp.o: ../math/main.cpp
CMakeFiles/math.dir/math/main.cpp.o: CMakeFiles/math.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yuna/cpp_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/math.dir/math/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/math.dir/math/main.cpp.o -MF CMakeFiles/math.dir/math/main.cpp.o.d -o CMakeFiles/math.dir/math/main.cpp.o -c /home/yuna/cpp_server/math/main.cpp

CMakeFiles/math.dir/math/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/math.dir/math/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/yuna/cpp_server/math/main.cpp > CMakeFiles/math.dir/math/main.cpp.i

CMakeFiles/math.dir/math/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/math.dir/math/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/yuna/cpp_server/math/main.cpp -o CMakeFiles/math.dir/math/main.cpp.s

CMakeFiles/math.dir/math/src/euler_angles.cpp.o: CMakeFiles/math.dir/flags.make
CMakeFiles/math.dir/math/src/euler_angles.cpp.o: ../math/src/euler_angles.cpp
CMakeFiles/math.dir/math/src/euler_angles.cpp.o: CMakeFiles/math.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yuna/cpp_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/math.dir/math/src/euler_angles.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/math.dir/math/src/euler_angles.cpp.o -MF CMakeFiles/math.dir/math/src/euler_angles.cpp.o.d -o CMakeFiles/math.dir/math/src/euler_angles.cpp.o -c /home/yuna/cpp_server/math/src/euler_angles.cpp

CMakeFiles/math.dir/math/src/euler_angles.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/math.dir/math/src/euler_angles.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/yuna/cpp_server/math/src/euler_angles.cpp > CMakeFiles/math.dir/math/src/euler_angles.cpp.i

CMakeFiles/math.dir/math/src/euler_angles.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/math.dir/math/src/euler_angles.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/yuna/cpp_server/math/src/euler_angles.cpp -o CMakeFiles/math.dir/math/src/euler_angles.cpp.s

CMakeFiles/math.dir/math/src/math_util.cpp.o: CMakeFiles/math.dir/flags.make
CMakeFiles/math.dir/math/src/math_util.cpp.o: ../math/src/math_util.cpp
CMakeFiles/math.dir/math/src/math_util.cpp.o: CMakeFiles/math.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yuna/cpp_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/math.dir/math/src/math_util.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/math.dir/math/src/math_util.cpp.o -MF CMakeFiles/math.dir/math/src/math_util.cpp.o.d -o CMakeFiles/math.dir/math/src/math_util.cpp.o -c /home/yuna/cpp_server/math/src/math_util.cpp

CMakeFiles/math.dir/math/src/math_util.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/math.dir/math/src/math_util.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/yuna/cpp_server/math/src/math_util.cpp > CMakeFiles/math.dir/math/src/math_util.cpp.i

CMakeFiles/math.dir/math/src/math_util.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/math.dir/math/src/math_util.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/yuna/cpp_server/math/src/math_util.cpp -o CMakeFiles/math.dir/math/src/math_util.cpp.s

CMakeFiles/math.dir/math/src/matrix4x3.cpp.o: CMakeFiles/math.dir/flags.make
CMakeFiles/math.dir/math/src/matrix4x3.cpp.o: ../math/src/matrix4x3.cpp
CMakeFiles/math.dir/math/src/matrix4x3.cpp.o: CMakeFiles/math.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yuna/cpp_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/math.dir/math/src/matrix4x3.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/math.dir/math/src/matrix4x3.cpp.o -MF CMakeFiles/math.dir/math/src/matrix4x3.cpp.o.d -o CMakeFiles/math.dir/math/src/matrix4x3.cpp.o -c /home/yuna/cpp_server/math/src/matrix4x3.cpp

CMakeFiles/math.dir/math/src/matrix4x3.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/math.dir/math/src/matrix4x3.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/yuna/cpp_server/math/src/matrix4x3.cpp > CMakeFiles/math.dir/math/src/matrix4x3.cpp.i

CMakeFiles/math.dir/math/src/matrix4x3.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/math.dir/math/src/matrix4x3.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/yuna/cpp_server/math/src/matrix4x3.cpp -o CMakeFiles/math.dir/math/src/matrix4x3.cpp.s

CMakeFiles/math.dir/math/src/quaternion.cpp.o: CMakeFiles/math.dir/flags.make
CMakeFiles/math.dir/math/src/quaternion.cpp.o: ../math/src/quaternion.cpp
CMakeFiles/math.dir/math/src/quaternion.cpp.o: CMakeFiles/math.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yuna/cpp_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/math.dir/math/src/quaternion.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/math.dir/math/src/quaternion.cpp.o -MF CMakeFiles/math.dir/math/src/quaternion.cpp.o.d -o CMakeFiles/math.dir/math/src/quaternion.cpp.o -c /home/yuna/cpp_server/math/src/quaternion.cpp

CMakeFiles/math.dir/math/src/quaternion.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/math.dir/math/src/quaternion.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/yuna/cpp_server/math/src/quaternion.cpp > CMakeFiles/math.dir/math/src/quaternion.cpp.i

CMakeFiles/math.dir/math/src/quaternion.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/math.dir/math/src/quaternion.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/yuna/cpp_server/math/src/quaternion.cpp -o CMakeFiles/math.dir/math/src/quaternion.cpp.s

CMakeFiles/math.dir/math/src/rotation_matrix.cpp.o: CMakeFiles/math.dir/flags.make
CMakeFiles/math.dir/math/src/rotation_matrix.cpp.o: ../math/src/rotation_matrix.cpp
CMakeFiles/math.dir/math/src/rotation_matrix.cpp.o: CMakeFiles/math.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yuna/cpp_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/math.dir/math/src/rotation_matrix.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/math.dir/math/src/rotation_matrix.cpp.o -MF CMakeFiles/math.dir/math/src/rotation_matrix.cpp.o.d -o CMakeFiles/math.dir/math/src/rotation_matrix.cpp.o -c /home/yuna/cpp_server/math/src/rotation_matrix.cpp

CMakeFiles/math.dir/math/src/rotation_matrix.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/math.dir/math/src/rotation_matrix.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/yuna/cpp_server/math/src/rotation_matrix.cpp > CMakeFiles/math.dir/math/src/rotation_matrix.cpp.i

CMakeFiles/math.dir/math/src/rotation_matrix.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/math.dir/math/src/rotation_matrix.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/yuna/cpp_server/math/src/rotation_matrix.cpp -o CMakeFiles/math.dir/math/src/rotation_matrix.cpp.s

CMakeFiles/math.dir/math/src/vector3.cpp.o: CMakeFiles/math.dir/flags.make
CMakeFiles/math.dir/math/src/vector3.cpp.o: ../math/src/vector3.cpp
CMakeFiles/math.dir/math/src/vector3.cpp.o: CMakeFiles/math.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yuna/cpp_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/math.dir/math/src/vector3.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/math.dir/math/src/vector3.cpp.o -MF CMakeFiles/math.dir/math/src/vector3.cpp.o.d -o CMakeFiles/math.dir/math/src/vector3.cpp.o -c /home/yuna/cpp_server/math/src/vector3.cpp

CMakeFiles/math.dir/math/src/vector3.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/math.dir/math/src/vector3.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/yuna/cpp_server/math/src/vector3.cpp > CMakeFiles/math.dir/math/src/vector3.cpp.i

CMakeFiles/math.dir/math/src/vector3.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/math.dir/math/src/vector3.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/yuna/cpp_server/math/src/vector3.cpp -o CMakeFiles/math.dir/math/src/vector3.cpp.s

CMakeFiles/math.dir/math/src/vector_num_type.cpp.o: CMakeFiles/math.dir/flags.make
CMakeFiles/math.dir/math/src/vector_num_type.cpp.o: ../math/src/vector_num_type.cpp
CMakeFiles/math.dir/math/src/vector_num_type.cpp.o: CMakeFiles/math.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yuna/cpp_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/math.dir/math/src/vector_num_type.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/math.dir/math/src/vector_num_type.cpp.o -MF CMakeFiles/math.dir/math/src/vector_num_type.cpp.o.d -o CMakeFiles/math.dir/math/src/vector_num_type.cpp.o -c /home/yuna/cpp_server/math/src/vector_num_type.cpp

CMakeFiles/math.dir/math/src/vector_num_type.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/math.dir/math/src/vector_num_type.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/yuna/cpp_server/math/src/vector_num_type.cpp > CMakeFiles/math.dir/math/src/vector_num_type.cpp.i

CMakeFiles/math.dir/math/src/vector_num_type.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/math.dir/math/src/vector_num_type.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/yuna/cpp_server/math/src/vector_num_type.cpp -o CMakeFiles/math.dir/math/src/vector_num_type.cpp.s

# Object files for target math
math_OBJECTS = \
"CMakeFiles/math.dir/math/main.cpp.o" \
"CMakeFiles/math.dir/math/src/euler_angles.cpp.o" \
"CMakeFiles/math.dir/math/src/math_util.cpp.o" \
"CMakeFiles/math.dir/math/src/matrix4x3.cpp.o" \
"CMakeFiles/math.dir/math/src/quaternion.cpp.o" \
"CMakeFiles/math.dir/math/src/rotation_matrix.cpp.o" \
"CMakeFiles/math.dir/math/src/vector3.cpp.o" \
"CMakeFiles/math.dir/math/src/vector_num_type.cpp.o"

# External object files for target math
math_EXTERNAL_OBJECTS =

math: CMakeFiles/math.dir/math/main.cpp.o
math: CMakeFiles/math.dir/math/src/euler_angles.cpp.o
math: CMakeFiles/math.dir/math/src/math_util.cpp.o
math: CMakeFiles/math.dir/math/src/matrix4x3.cpp.o
math: CMakeFiles/math.dir/math/src/quaternion.cpp.o
math: CMakeFiles/math.dir/math/src/rotation_matrix.cpp.o
math: CMakeFiles/math.dir/math/src/vector3.cpp.o
math: CMakeFiles/math.dir/math/src/vector_num_type.cpp.o
math: CMakeFiles/math.dir/build.make
math: CMakeFiles/math.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/yuna/cpp_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Linking CXX executable math"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/math.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/math.dir/build: math
.PHONY : CMakeFiles/math.dir/build

CMakeFiles/math.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/math.dir/cmake_clean.cmake
.PHONY : CMakeFiles/math.dir/clean

CMakeFiles/math.dir/depend:
	cd /home/yuna/cpp_server/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/yuna/cpp_server /home/yuna/cpp_server /home/yuna/cpp_server/build /home/yuna/cpp_server/build /home/yuna/cpp_server/build/CMakeFiles/math.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/math.dir/depend

