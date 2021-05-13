
# Project

Simple boilerplate cmake project. Features: boost program options, cppunit, cmake. Clone, rename and you're ready to go in 1 minute.

Features:
- gtest
- cppunit
- boost program options
- boost process
- using static library (if necessary)
- easy to clone to your project by simply renaming the folder
- clang formatting template for VSCode based on Google
- make tasks to be able to debug project using VSCode

- get started in 1 minute

*Note*: this project is supporting "Unix Makefiles" only, no ninja support due to some
potential limitations on average system.

# Pre-requisites

c++ compiler
cmake
boost
cppunit
gtest
a lot of free time

# Installation

After cloning the project rename a parent folder and delete .git derectory.
Run script:
./rename_project.sh

After this step all local names will be set to match a directory name you put this
project in.

# Running

To build a project you can use prepare_build.sh script.

- Clean
./prepare_build.sh -c

- Build
./prepare_build.sh run

- Test using cppunit
./prepare_build.sh test

- Test using gtest
./prepare_build.sh gtest

# Using static library (default)

To disable static library comment out line:
set(USING_LIB 1) 
in CMakeLists.txt

# Todo

Just add some source files to ./src folder, build, debug and enjoy.

