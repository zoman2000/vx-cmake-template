#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/process.hpp>

#include <gtest/gtest.h>

namespace bp = boost::process;
using namespace std;

// Note: we don't need to include RegisteredTest

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  int result = RUN_ALL_TESTS();

  // boost::filesystem::path full_path(boost::filesystem::current_path());
  // std::cout << "Current path is : " << full_path << std::endl;

  // // run main program with some options
  // bp::child server("./test_gtest", "9998", "3");
  // std::this_thread::sleep_for(std::chrono::milliseconds(500));
  // server.terminate();

  return result;
}
