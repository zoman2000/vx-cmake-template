/**
 * @file SimpleTest.cpp
 * @author Alexander Kuznetsov <alx.kuzza@gmail.com>
 * @brief Very basic gtest implementation.
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

 * 
 */
#include <iostream>

#include <gtest/gtest.h>
#include <gtest/gtest-matchers.h>
#include <gmock/gmock.h>

#include "../src/Sample.h"

using namespace std;

using ::testing::Ge;
using ::testing::Le;
using ::testing::MatchesRegex;
using ::testing::StartsWith;

/**
 * @brief sample fixture class
 * 
 */
struct SampleFix : testing::Test {
  
  shared_ptr<Sample> ooo; //!< class instance 
  SampleFix() {
    ooo.reset(new Sample);
  }
  /// Initialize test fixture
  void SetUp() override {
    // cout << "setup" << endl;
  }
  /// test cleanup
  void TearDown() override {
    // cout << "teardown final val =" << ooo->val << endl;
  } 
};

// testing additon, considering that a constructor is initializing "val" to 0
TEST_F(SampleFix, Add) {
  ooo->add(2);
  EXPECT_EQ(2, ooo->val) << "should be 2";
}

// ---- parameters with the state

// 
/**
 * @brief state class is necessary to run multiple parameterized test cases (see below)
 * 
 */
struct SampleFixState {
  unsigned int start; //!< assign to Sample().val. Initial value
  unsigned int val;   //!< value to subtract from val
  unsigned int endval; //!< final expected value
  /// this method allows to show an extended human-readable information about the state data
  friend std::ostream& operator << (std::ostream&os, const SampleFixState& o) {
    return os << "start: " << o.start << " val: " << o.val << " endval: " << o.endval;
  }
};

/**
 * @brief fixture class to use a state
 * 
 */
struct SampleFixOP : SampleFix, testing::WithParamInterface<SampleFixState> {
  SampleFixOP() {
    ooo->val = GetParam().start;
  }
};

/// define test case that uses a fixture.
TEST_P(SampleFixOP,Result) {
  auto as = GetParam();
  ooo->sub(as.val);
  EXPECT_EQ(as.endval, ooo->val);
}

/// run multiple tests using parameters
INSTANTIATE_TEST_SUITE_P(SampleSub, SampleFixOP, testing::Values(
  SampleFixState{ 0, 0, 0},
  SampleFixState{ 1, 1, 0},
  SampleFixState{ 1, 2, 0},
  SampleFixState{ 10, 2, 8}
));

//******* extended true/false

testing::AssertionResult IsTwo(int v) {
  if (v == 2)
    return testing::AssertionSuccess() << " it is two, yay";
  else
    return testing::AssertionFailure() << " not two";
}

testing::AssertionResult IsThree(int v) {
  if (v == 3)
    return testing::AssertionSuccess() << " it is three, yay";
  else
    return testing::AssertionFailure() << " not three";
}

TEST_F(SampleFix, Two) {
  ooo->add(2);
  EXPECT_TRUE(IsTwo(ooo->val));
}

TEST_F(SampleFix, Three) {
  ooo->add(3);
  EXPECT_TRUE(IsThree(ooo->val));
}

//****** predicates, i.e. pred(param)

/// will utilize operator () declared in class Sample and pass 0 as a parameter
TEST_F(SampleFix, Predicate0) {
  ASSERT_PRED1(*ooo, 0);
}

/// will utilize operator () declared in class Sample and pass 2 as a parameter
TEST_F(SampleFix, Predicate2) {
  ooo->add(2);
  ASSERT_PRED1(*ooo, 2);
}

//*** string match (just a few examples)

/// Match regex
TEST_F(SampleFix, OOOMatch) {
  string val = "Hello World!";
  ASSERT_THAT(val, ::testing::MatchesRegex(".*ello.*"));
}

/// Match start of the string
TEST_F(SampleFix, OOOMatch2) {
  string val = "Hello World!";
  ASSERT_THAT(val, ::testing::StartsWith("Hello"));
}
