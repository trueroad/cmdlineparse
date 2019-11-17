//
// One header file Commandline Parse for C++11
// https://github.com/trueroad/cmdlineparse
//
// Simple Sample
//
// Copyright (C) 2016, 2017, 2019 Masamichi Hosoda. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED.
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
// OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
// SUCH DAMAGE.
//

#include <iostream>
#include "cmdlineparse.hh"

int main (int argc, char *argv[])
{
  // Prepare parser instance
  cmdlineparse::parser cmd;

  // Prepare variable to store the parse result
  std::string str;
  bool flag;

  // Set version string for `--help' and `--version' etc.
  cmd.set_version_string (
"One header file Commandline Parse for C++11: Simple Sample\n"
"Copyright (C) 2016-2019 Masamichi Hosoda. All rights reserved.\n"
                          );

  // Add `--help' and `--version' options
  cmd.add_default ();

  // Add a string option
  //   1st arg: short option name
  //   2nd arg: long option name
  //   3rd arg: a pointer to store the parse result
  //   4th arg: default value
  //   5th arg: description for `--help'
  //            Four spaces indent is recommended.
  cmd.add_string ('s', "string", &str, "foobar",
                  "    Specify string option.");

  // Add a flag option
  //   1st arg: short option name
  //   2nd arg: long option name
  //   3rd arg: pointer to store the parse result
  //   4th arg: description for `--help'
  //            This parser doesn't warp text automatically.
  //            Four spaces indent is recommended.
  cmd.add_flag ('f', "flag", &flag,
                "    This is a flag.\n"
                "    Second line.\n"
                "    This parser doesn't warp text automatically.");

  // Parse
  if (!cmd.parse (argc, argv))
    return 0;

  // Output results
  std::cout << "string option = \"" << str << "\"" << std::endl;
  std::cout << "flag = " << ( flag ? "true" : "false" ) << std::endl;

  return 0;
}
