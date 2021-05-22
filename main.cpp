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

#include "stdafx.h"

#include <boost/lexical_cast.hpp>
#include <boost/program_options.hpp>

#include <Template.h>

using namespace std;
using namespace boost::property_tree;
using namespace boost::archive;

using namespace boost::program_options;

int main(int argc, char **argv) {
  //+++ start of options
  options_description settings{"Options"};
  // clang-format off
  settings.add_options()
    ("help", "Help")
    ("port,p", value<int>()->default_value(9998), "Port")
    ("host,h", value<string>()->default_value("localhost"), "Host")
    ;
  // clang-format on

  // Define positional arguments
  // allows to start
  // main localhost 1234
  // vs
  // main -h localhost -p 1234 
  positional_options_description pdesc;
  pdesc.add("host", 1);
  pdesc.add("port", 1);

  command_line_parser parser{argc, argv};
  parser.options(settings).allow_unregistered().style(
      command_line_style::default_style |
      command_line_style::allow_slash_for_short);
  parsed_options parsed_options = parser.run();

  variables_map vars;
  try {
    store(parsed_options, vars);
  }
  catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
    exit(1);
  }
  notify(vars);

  if (vars.count("help")) {
    cout << settings << endl;
    return 0;
  }

  //--- End of options

  cout << "starting using host=" << vars["host"].as<string>() << " port=" << vars["port"].as<int>() << "\n";

  Template temp;
  temp.Hello();
}
