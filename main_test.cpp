/*
 * Copyright 2018 Alexander Kuznetsov <alx.kuzza@gmail.com>
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

#include <cppunit/TestCaller.h>
#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/process.hpp>

#include "test/MainTest.h"

using namespace std;
namespace bp = boost::process;

// if START_MAIN is defined, main program will be started before running tests
//#define START_MAIN

#define FOCUS(x) \
  if (x::suite()->getName() == focus) runner.addTest(x::suite());

int main(int argc, char **argv) {
  CppUnit::TextUi::TestRunner runner;

  vector<string> vfocus;
  bool start_servers = true;
  for (auto i = 1; i < argc; i++) {
    auto val = argv[i];
    if (string("-") == val) {
      start_servers = false;
      continue;
    }
    vfocus.push_back(val);
  }

  // tests from the registry
  if (vfocus.empty()) {
    CppUnit::TestFactoryRegistry &registry =
        CppUnit::TestFactoryRegistry::getRegistry();
    runner.addTest(registry.makeTest());
  }
  // tests defined from the command line
  else {
    while (!vfocus.empty()) {
      auto focus = vfocus[vfocus.size() - 1];
      vfocus.pop_back();
      FOCUS(MainTest)
    }
  }

#ifdef START_MAIN
  // starting main program with 2 parameters as an example: 9998 and localhost.
  bp::child server("./vx-cmake-template", "localhost", "9998");
  // wait main process to start
  std::this_thread::sleep_for(std::chrono::milliseconds(300));
#endif

  int result = (runner.run("",
                           /*wait*/ false,
                           /*printResult*/ true,
                           /*printProgress*/ true))
                   ? 0
                   : 1;

#ifdef START_MAIN
  server.terminate();
#endif

  return result;
}
