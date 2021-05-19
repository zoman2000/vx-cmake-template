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

#ifndef _SERVER_TEST_H
#define _SERVER_TEST_H

#include <cppunit/TestCaller.h>
#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include "../stdafx.h"

using namespace std;

/**
 * @brief CPPUNIT test suite
 * 
 */
class MainTest : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(MainTest);

  CPPUNIT_TEST(testOne);

  CPPUNIT_TEST_SUITE_END();

 private:
 public:
  MainTest();
  ~MainTest();

  /// setup tests
  void setUp() override;

  /// cleanup after runnint a test
  void tearDown() override;

  /// sample test case
  void testOne();
};

#endif
