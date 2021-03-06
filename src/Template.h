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

#ifndef _TEMPLATE_H
#define _TEMPLATE_H

#include <boost/property_tree/ptree.hpp>
#include <memory>

using namespace boost::property_tree;
using namespace std;

/**
 * @brief Sample dummy class
 * 
 */
class Template {
 public:
  Template();
  ~Template();
  /// Sample function
  void Hello();
};

#endif
