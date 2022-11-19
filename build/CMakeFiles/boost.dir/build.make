# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_SOURCE_DIR = /home/xiaobo/myspace/owner/mynetwork/guodong

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/xiaobo/myspace/owner/mynetwork/guodong/build

# Utility rule file for boost.

# Include the progress variables for this target.
include CMakeFiles/boost.dir/progress.make

CMakeFiles/boost: CMakeFiles/boost-complete


CMakeFiles/boost-complete: ../deps/src/boost-stamp/boost-install
CMakeFiles/boost-complete: ../deps/src/boost-stamp/boost-mkdir
CMakeFiles/boost-complete: ../deps/src/boost-stamp/boost-download
CMakeFiles/boost-complete: ../deps/src/boost-stamp/boost-update
CMakeFiles/boost-complete: ../deps/src/boost-stamp/boost-patch
CMakeFiles/boost-complete: ../deps/src/boost-stamp/boost-configure
CMakeFiles/boost-complete: ../deps/src/boost-stamp/boost-build
CMakeFiles/boost-complete: ../deps/src/boost-stamp/boost-install
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/xiaobo/myspace/owner/mynetwork/guodong/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Completed 'boost'"
	/usr/bin/cmake -E make_directory /home/xiaobo/myspace/owner/mynetwork/guodong/build/CMakeFiles
	/usr/bin/cmake -E touch /home/xiaobo/myspace/owner/mynetwork/guodong/build/CMakeFiles/boost-complete
	/usr/bin/cmake -E touch /home/xiaobo/myspace/owner/mynetwork/guodong/deps/src/boost-stamp/boost-done

../deps/src/boost-stamp/boost-install: ../deps/src/boost-stamp/boost-build
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/xiaobo/myspace/owner/mynetwork/guodong/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "No install step for 'boost'"
	cd /home/xiaobo/myspace/owner/mynetwork/guodong/deps/src/boost && /usr/bin/cmake -E echo_append
	cd /home/xiaobo/myspace/owner/mynetwork/guodong/deps/src/boost && /usr/bin/cmake -E touch /home/xiaobo/myspace/owner/mynetwork/guodong/deps/src/boost-stamp/boost-install

../deps/src/boost-stamp/boost-mkdir:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/xiaobo/myspace/owner/mynetwork/guodong/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Creating directories for 'boost'"
	/usr/bin/cmake -E make_directory /home/xiaobo/myspace/owner/mynetwork/guodong/deps/src/boost
	/usr/bin/cmake -E make_directory /home/xiaobo/myspace/owner/mynetwork/guodong/deps/src/boost
	/usr/bin/cmake -E make_directory /home/xiaobo/myspace/owner/mynetwork/guodong/deps
	/usr/bin/cmake -E make_directory /home/xiaobo/myspace/owner/mynetwork/guodong/deps/tmp
	/usr/bin/cmake -E make_directory /home/xiaobo/myspace/owner/mynetwork/guodong/deps/src/boost-stamp
	/usr/bin/cmake -E make_directory /home/xiaobo/myspace/owner/mynetwork/guodong/deps/src
	/usr/bin/cmake -E make_directory /home/xiaobo/myspace/owner/mynetwork/guodong/deps/src/boost-stamp
	/usr/bin/cmake -E touch /home/xiaobo/myspace/owner/mynetwork/guodong/deps/src/boost-stamp/boost-mkdir

../deps/src/boost-stamp/boost-download: ../deps/src/boost-stamp/boost-urlinfo.txt
../deps/src/boost-stamp/boost-download: ../deps/src/boost-stamp/boost-mkdir
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/xiaobo/myspace/owner/mynetwork/guodong/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Performing download step (download, verify and extract) for 'boost'"
	cd /home/xiaobo/myspace/owner/mynetwork/guodong/deps/src && /usr/bin/cmake -P /home/xiaobo/myspace/owner/mynetwork/guodong/deps/src/boost-stamp/download-boost.cmake
	cd /home/xiaobo/myspace/owner/mynetwork/guodong/deps/src && /usr/bin/cmake -P /home/xiaobo/myspace/owner/mynetwork/guodong/deps/src/boost-stamp/verify-boost.cmake
	cd /home/xiaobo/myspace/owner/mynetwork/guodong/deps/src && /usr/bin/cmake -P /home/xiaobo/myspace/owner/mynetwork/guodong/deps/src/boost-stamp/extract-boost.cmake
	cd /home/xiaobo/myspace/owner/mynetwork/guodong/deps/src && /usr/bin/cmake -E touch /home/xiaobo/myspace/owner/mynetwork/guodong/deps/src/boost-stamp/boost-download

../deps/src/boost-stamp/boost-update: ../deps/src/boost-stamp/boost-download
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/xiaobo/myspace/owner/mynetwork/guodong/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "No update step for 'boost'"
	/usr/bin/cmake -E echo_append
	/usr/bin/cmake -E touch /home/xiaobo/myspace/owner/mynetwork/guodong/deps/src/boost-stamp/boost-update

../deps/src/boost-stamp/boost-patch: ../deps/src/boost-stamp/boost-download
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/xiaobo/myspace/owner/mynetwork/guodong/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "No patch step for 'boost'"
	/usr/bin/cmake -E echo_append
	/usr/bin/cmake -E touch /home/xiaobo/myspace/owner/mynetwork/guodong/deps/src/boost-stamp/boost-patch

../deps/src/boost-stamp/boost-configure: ../deps/tmp/boost-cfgcmd.txt
../deps/src/boost-stamp/boost-configure: ../deps/src/boost-stamp/boost-update
../deps/src/boost-stamp/boost-configure: ../deps/src/boost-stamp/boost-patch
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/xiaobo/myspace/owner/mynetwork/guodong/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Performing configure step for 'boost'"
	cd /home/xiaobo/myspace/owner/mynetwork/guodong/deps/src/boost && /usr/bin/cmake -P /home/xiaobo/myspace/owner/mynetwork/guodong/deps/src/boost-stamp/boost-configure-.cmake
	cd /home/xiaobo/myspace/owner/mynetwork/guodong/deps/src/boost && /usr/bin/cmake -E touch /home/xiaobo/myspace/owner/mynetwork/guodong/deps/src/boost-stamp/boost-configure

../deps/src/boost-stamp/boost-build: ../deps/src/boost-stamp/boost-configure
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/xiaobo/myspace/owner/mynetwork/guodong/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Performing build step for 'boost'"
	cd /home/xiaobo/myspace/owner/mynetwork/guodong/deps/src/boost && /usr/bin/cmake -P /home/xiaobo/myspace/owner/mynetwork/guodong/deps/src/boost-stamp/boost-build-.cmake
	cd /home/xiaobo/myspace/owner/mynetwork/guodong/deps/src/boost && /usr/bin/cmake -E touch /home/xiaobo/myspace/owner/mynetwork/guodong/deps/src/boost-stamp/boost-build

boost: CMakeFiles/boost
boost: CMakeFiles/boost-complete
boost: ../deps/src/boost-stamp/boost-install
boost: ../deps/src/boost-stamp/boost-mkdir
boost: ../deps/src/boost-stamp/boost-download
boost: ../deps/src/boost-stamp/boost-update
boost: ../deps/src/boost-stamp/boost-patch
boost: ../deps/src/boost-stamp/boost-configure
boost: ../deps/src/boost-stamp/boost-build
boost: CMakeFiles/boost.dir/build.make

.PHONY : boost

# Rule to build all files generated by this target.
CMakeFiles/boost.dir/build: boost

.PHONY : CMakeFiles/boost.dir/build

CMakeFiles/boost.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/boost.dir/cmake_clean.cmake
.PHONY : CMakeFiles/boost.dir/clean

CMakeFiles/boost.dir/depend:
	cd /home/xiaobo/myspace/owner/mynetwork/guodong/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/xiaobo/myspace/owner/mynetwork/guodong /home/xiaobo/myspace/owner/mynetwork/guodong /home/xiaobo/myspace/owner/mynetwork/guodong/build /home/xiaobo/myspace/owner/mynetwork/guodong/build /home/xiaobo/myspace/owner/mynetwork/guodong/build/CMakeFiles/boost.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/boost.dir/depend
