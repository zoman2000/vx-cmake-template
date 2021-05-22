/*
 * Copyright 2019 Alexander Kuznetsov <alx.kuzza@replaytrader.com>
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

#ifndef _SAMPLE_H
#define _SAMPLE_H

#include <boost/property_tree/ptree.hpp>
#include <memory>

using namespace boost::property_tree;
using namespace std;

/**
 * @brief Class to test using GTEST.
 * 
 * This class mimics a very basic "store" that allows to add and subtract a value.
 */
class Sample {
public:
  
  unsigned int val; //!< value that could be >= 0

  Sample();
  ~Sample();

  /**
   * @brief Add a number.
   * 
   * @param v value to add
   */
  void add(unsigned int v);

  /**
   * @brief Subtract a number.
   * 
   * @param v valud to sutract
   */
  void sub(unsigned int v);

  /// sample functor to use as a predicate in GTEST
  bool operator()(unsigned int v1);
};

#endif
