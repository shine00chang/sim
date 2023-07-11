# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.26

# Default target executed when no arguments are given to make.
default_target: all
.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:

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
CMAKE_COMMAND = /opt/homebrew/Cellar/cmake/3.26.3/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/Cellar/cmake/3.26.3/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/shinechang/Documents/CS/Self2023/physics

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/shinechang/Documents/CS/Self2023/physics

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake cache editor..."
	/opt/homebrew/Cellar/cmake/3.26.3/bin/ccmake -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache
.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/opt/homebrew/Cellar/cmake/3.26.3/bin/cmake --regenerate-during-build -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache
.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /Users/shinechang/Documents/CS/Self2023/physics/CMakeFiles /Users/shinechang/Documents/CS/Self2023/physics//CMakeFiles/progress.marks
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /Users/shinechang/Documents/CS/Self2023/physics/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean
.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named prog

# Build rule for target.
prog: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 prog
.PHONY : prog

# fast build rule for target.
prog/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/prog.dir/build.make CMakeFiles/prog.dir/build
.PHONY : prog/fast

src/app.o: src/app.cpp.o
.PHONY : src/app.o

# target to build an object file
src/app.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/prog.dir/build.make CMakeFiles/prog.dir/src/app.cpp.o
.PHONY : src/app.cpp.o

src/app.i: src/app.cpp.i
.PHONY : src/app.i

# target to preprocess a source file
src/app.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/prog.dir/build.make CMakeFiles/prog.dir/src/app.cpp.i
.PHONY : src/app.cpp.i

src/app.s: src/app.cpp.s
.PHONY : src/app.s

# target to generate assembly for a file
src/app.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/prog.dir/build.make CMakeFiles/prog.dir/src/app.cpp.s
.PHONY : src/app.cpp.s

src/body.o: src/body.cpp.o
.PHONY : src/body.o

# target to build an object file
src/body.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/prog.dir/build.make CMakeFiles/prog.dir/src/body.cpp.o
.PHONY : src/body.cpp.o

src/body.i: src/body.cpp.i
.PHONY : src/body.i

# target to preprocess a source file
src/body.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/prog.dir/build.make CMakeFiles/prog.dir/src/body.cpp.i
.PHONY : src/body.cpp.i

src/body.s: src/body.cpp.s
.PHONY : src/body.s

# target to generate assembly for a file
src/body.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/prog.dir/build.make CMakeFiles/prog.dir/src/body.cpp.s
.PHONY : src/body.cpp.s

src/collision.o: src/collision.cpp.o
.PHONY : src/collision.o

# target to build an object file
src/collision.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/prog.dir/build.make CMakeFiles/prog.dir/src/collision.cpp.o
.PHONY : src/collision.cpp.o

src/collision.i: src/collision.cpp.i
.PHONY : src/collision.i

# target to preprocess a source file
src/collision.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/prog.dir/build.make CMakeFiles/prog.dir/src/collision.cpp.i
.PHONY : src/collision.cpp.i

src/collision.s: src/collision.cpp.s
.PHONY : src/collision.s

# target to generate assembly for a file
src/collision.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/prog.dir/build.make CMakeFiles/prog.dir/src/collision.cpp.s
.PHONY : src/collision.cpp.s

src/main.o: src/main.cpp.o
.PHONY : src/main.o

# target to build an object file
src/main.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/prog.dir/build.make CMakeFiles/prog.dir/src/main.cpp.o
.PHONY : src/main.cpp.o

src/main.i: src/main.cpp.i
.PHONY : src/main.i

# target to preprocess a source file
src/main.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/prog.dir/build.make CMakeFiles/prog.dir/src/main.cpp.i
.PHONY : src/main.cpp.i

src/main.s: src/main.cpp.s
.PHONY : src/main.s

# target to generate assembly for a file
src/main.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/prog.dir/build.make CMakeFiles/prog.dir/src/main.cpp.s
.PHONY : src/main.cpp.s

src/renderer.o: src/renderer.cpp.o
.PHONY : src/renderer.o

# target to build an object file
src/renderer.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/prog.dir/build.make CMakeFiles/prog.dir/src/renderer.cpp.o
.PHONY : src/renderer.cpp.o

src/renderer.i: src/renderer.cpp.i
.PHONY : src/renderer.i

# target to preprocess a source file
src/renderer.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/prog.dir/build.make CMakeFiles/prog.dir/src/renderer.cpp.i
.PHONY : src/renderer.cpp.i

src/renderer.s: src/renderer.cpp.s
.PHONY : src/renderer.s

# target to generate assembly for a file
src/renderer.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/prog.dir/build.make CMakeFiles/prog.dir/src/renderer.cpp.s
.PHONY : src/renderer.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... rebuild_cache"
	@echo "... prog"
	@echo "... src/app.o"
	@echo "... src/app.i"
	@echo "... src/app.s"
	@echo "... src/body.o"
	@echo "... src/body.i"
	@echo "... src/body.s"
	@echo "... src/collision.o"
	@echo "... src/collision.i"
	@echo "... src/collision.s"
	@echo "... src/main.o"
	@echo "... src/main.i"
	@echo "... src/main.s"
	@echo "... src/renderer.o"
	@echo "... src/renderer.i"
	@echo "... src/renderer.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

