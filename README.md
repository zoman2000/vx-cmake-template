# Disclaimer

This project is provided for educational and demonstration purpose only on the "AS-IS" basis. Sample code could
be incomplete or outdated as well as provided readme files and comments.
Setup could not be considered as "production ready", may require additional work to be completed after 
a project is cloned locally.

# Project

(Original URL: https://gitlab.com/alexsmit/vx-cmake-template)

Simple boilerplate cmake project. Features: boost program options, doxygen, gtest, cppunit(optional), cmake. Clone, rename and you're ready to go in 1 minute.

Features:
- gtest
- cppunit
- doxygen
- boost program options
- boost process
- using static library (if necessary)
- easy to clone to your project by simply renaming the folder
- clang formatting template for VSCode based on Google
- make tasks to be able to debug project using VSCode
- using lcov to generate coverage report
- create RPM or DEB package

- get started in 1 minute

*Note*: this project is supporting "Unix Makefiles" only, no ninja support due to some
potential limitations on average system.

Supported platforms: Linux.

# Pre-requisites

c++ compiler
cmake
boost
cppunit
gtest
lcov (optional)
rpm/deb build utilities (optional)
a lot of free time

# IDE
This project has everything ready to use VS Code to compile and run this project.

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

# Doxygen
If installed to build a documentation navigate to build folder and run
```
make doc
```

# Using static/shared library (default)

To disable building libraries comment this line:
set(USING_LIB 1) 
in CMakeLists.txt

To link using shared library comment this line:
set(LINK_TYPE static)
in CMakeLists.txt

# Coverate using lcov

Build with coverage enabled: 

```./prepare_build.sh cov```

To generate a report run:

```run-coverage ```

# RPM/DEB

To create a system package simply run:

```
./prepare_build.sh prod
cd build
make package
``` 

Version of the package will be set by the latest tag in form X.Y.Z where:
X - major version
Y - minor version
Z - patch

If there is no tag a version will be set as 0.0.1

NOTE: a valid configuration is necessary to be able to sign a generated RPM file.
# Todo

Just add some source files to ./src folder, build, debug and enjoy.
