/*
 * Copyright 2019 Alexander Kuznetsov <alx.kuzza@gmail.com>
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

#include <string>
#include <iostream>
#define BOOST_SPIRIT_THREADSAFE

#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>

#include "Sample.h"

using namespace std;
using namespace boost::property_tree;
using namespace boost::archive;

/*  
  Sample class to run simple test cases using gtest
  Tests will be "evaluating" that we can add, subtract and initialize class with zero
  Operator () is being used to run test cases using predicates
*/

Sample::Sample() : val(0) {}

Sample::~Sample() {}

void Sample::add(int v) { val += v; }

void Sample::sub(int v) {
  if (v <= 0) return;
  if (v > val) v = val;
  val -= v;
}

bool Sample::operator()(int v1) { return val == v1; }
