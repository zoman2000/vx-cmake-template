/**
 * @file main_gtest.cpp
 * @author Alexander Kuznetsov (alx.kuzza@gmail.com)
 * @brief gtest main file
 * @version 0.1
 * @date 2021-06-15
 * 
 * @copyright Copyright (c) 2021 Alexander Kuznetsov <alx.kuzza@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
