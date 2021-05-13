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

struct SampleFix : testing::Test {
  shared_ptr<Sample> ooo;
  SampleFix() {
    ooo.reset(new Sample);
  }
  void SetUp() override {
    // cout << "setup" << endl;
  }
  void TearDown() override {
    // cout << "teardown final val =" << ooo->val << endl;
  } 
};

TEST_F(SampleFix, Add) {
  ooo->add(2);
  EXPECT_EQ(2, ooo->val) << "should be 2";
}

// ---- parameters with the state

struct SampleFixState {
  int start;
  int val;
  int endval;
  friend std::ostream& operator << (std::ostream&os, const SampleFixState& o) {
    return os << "start: " << o.start << " val: " << o.val << " endval: " << o.endval;
  }
};

struct SampleFixOP : SampleFix, testing::WithParamInterface<SampleFixState> {
  SampleFixOP() {
    ooo->val = GetParam().start;
  }
};

TEST_P(SampleFixOP,Result) {
  auto as = GetParam();
  ooo->sub(as.val);
  EXPECT_EQ(as.endval, ooo->val);
}

INSTANTIATE_TEST_CASE_P(SampleSub, SampleFixOP, testing::Values(
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

TEST_F(SampleFix, Predicate0) {
  ASSERT_PRED1(*ooo, 0);
}

TEST_F(SampleFix, Predicate2) {
  ooo->add(2);
  ASSERT_PRED1(*ooo, 2);
}

//*** string match (just a few examples)

TEST_F(SampleFix, OOOMatch) {
  string val = "Hello World!";
  ASSERT_THAT(val, ::testing::MatchesRegex(".*ello.*"));
}

TEST_F(SampleFix, OOOMatch2) {
  string val = "Hello World!";
  ASSERT_THAT(val, ::testing::StartsWith("Hello"));
}
