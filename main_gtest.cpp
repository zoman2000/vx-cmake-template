#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/process.hpp>

#include <gtest/gtest.h>

// if START_MAIN is defined, main program will be started before running tests
//#define START_MAIN

namespace bp = boost::process;
using namespace std;

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);

#ifdef START_MAIN
  // starting main program with 2 parameters as an example: localhost and 9998
  bp::child server("./vx-cmake-template", "localhost", "9998");
  // wait main process to start
  std::this_thread::sleep_for(std::chrono::milliseconds(300));
#endif

  int result = RUN_ALL_TESTS();

#ifdef START_MAIN
  server.terminate();
#endif

  return result;
}
