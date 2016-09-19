//
// One header file Commandline Parse for C++11
// https://github.com/trueroad/cmdlineparse/
//
// Advanced Sample
//
// Copyright (C) 2016 Masamichi Hosoda. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY AUTHOR AND CONTRIBUTORS ``AS IS'' AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED.  IN NO EVENT SHALL AUTHOR OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
// OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
// SUCH DAMAGE.
//

#include "config.h"

#include <iostream>
#include "cmdlineparse.hh"

namespace
{
  bool callback_function (const std::string &optarg);
}

int main (int argc, char *argv[])
{
  // Prepare parser instance
  cmdlineparse::parser cmd;

  // Prepare variable to store the parse result
  std::string str, str2;
  bool flag, flag2;

  // Set version string for `help' and `version' etc.
  cmd.set_version_string (
"One header file Commandline Parse for C++11: Advanced Sample\n"
"Copyright (C) 2016 Masamichi Hosoda. All rights reserved.\n"
                          );

  // Set usage_unamed_opts (options without names e.g. file names)
  // for usage in `help'
  cmd.set_usage_unamed_opts ("FILES");

  // Add `help' and `version' options
  cmd.add_default ();

  // Add a string option
  //   1st arg: short option name
  //   2nd arg: long option name
  //   3rd arg: a pointer to store the parse result
  //   4th arg: default value
  //   5th arg: description for `help'
  cmd.add_string ('s', "string", &str, "foo",
                  cmd.d_indent + "Specify string option.");

  // Add another string option
  //   2nd arg: this option doesn't have a long name
  //   6th arg: typestr for `help'
  //   7th arg: option group name for `help'
  cmd.add_string ('t', "", &str2, "bar",
                  cmd.d_indent + "Specify string option 2.",
                  "FILENAME",
                  "Group1");

  // Add a flag option
  //   1st arg: short option name
  //   2nd arg: long option name
  //   3rd arg: pointer to store the parse result
  //   4th arg: description for `help'
  cmd.add_flag ('f', "flag", &flag,
                cmd.d_indent + "This is a flag.\n" +
                cmd.d_indent + "Second line.\n" +
                cmd.d_indent + "This parser doesn't warp text automatically.");

  // Add another flag option
  //   1st arg: this option doesn't have a short name
  //   5th arg: option group name for `help'
  cmd.add_flag (0, "flag-another", &flag2,
                cmd.d_indent + "This is a flag 2.",
                "Group1");

  // Add option handler
  //   1st arg: short option name
  //   2nd arg: long option name
  //   3rd arg: {no|required|optional}_argument like getopt_long ()
  //   4th arg: option handler (lambda expression)
  //   5th arg: description for `help'
  //   6th arg: typestr for `help'
  cmd.add_handler ('H', "handler",
                   cmdlineparse::arg_mode::optional_argument,
                   [](const std::string &optarg)
                   {
                     std::cout << "This is handler \""
                               << optarg << "\"" << std::endl;
                     return true;
                   },
                   cmd.d_indent + "This is option handler",
                   "OPTION");

  // Add another option handler
  //   4th arg: option handler (function)
  //   6th arg: typestr for `help'
  //   7th arg: header for `help'
  //   8th arg: option group name for `help'
  cmd.add_handler ('F', "callback-func",
                   cmdlineparse::arg_mode::required_argument,
                   callback_function,
                   cmd.d_indent + "This is callback hander",
                   "OPTARG",
                   cmd.h_space + "(OPTARG is required)",
                   "Group1");

  // Add description only
  //   1st arg: this option doesn't have a short name
  //   2nd arg: this option doesn't have a long name
  //   3rd arg: this option doesn't require argument
  //   4th arg: description for `help'
  cmd.add_description (0, "", cmdlineparse::arg_mode::no_argument,
                       "Description only\n" +
                       cmd.h_indent + "description only");

  // Add another description only
  //   5th arg: this option doesn't have typestr
  //   6th arg: header for `help'
  //   7th arg: option group name for `help'
  cmd.add_description (0, "", cmdlineparse::arg_mode::no_argument,
                       cmd.d_indent + "description only 2 - first line\n" +
                       cmd.d_indent + "description only 2 - second line",
                       "",
                       "Description 2",
                       "Group1");

  // Add option handler (sample for abort parsing)
  cmd.add_handler ('a', "abort", cmdlineparse::arg_mode::required_argument,
                   [](const std::string &optarg)
                   {
                     std::cout << "This is abort handler" << std::endl;
                     if (optarg == "true")
                       {
                         std::cout << "  Abort parsing" << std::endl;
                         return false;  // Abort
                       }
                     else if (optarg == "false")
                       {
                         std::cout << "  Continue parsing" << std::endl;
                         return true;  // Continue
                       }
                     std::cout << "Unknown optarg \"" << optarg << "\""
                               << std::endl << "  Continue..." << std::endl;
                     return true;  // Continue
                   },
                   cmd.d_indent + "This option aborts parsing\n" +
                   cmd.d_indent + "If FLAG=true, abort parsing",
                   "FLAG",
                   cmd.h_space + "(possible value=true, false)",
                   "Group2");

  // Parse
  if (!cmd.parse (argc, argv))
    {
      std::cout << "Abort reason is ";
      switch (cmd.get_abort_reason ())
        {
        case cmdlineparse::abort_reason::option_handler:
          std::cout << "option_handler -- "
                    << cmd.get_abort_option () << std::endl;
          return 0;
        case cmdlineparse::abort_reason::no_abort:
          std::cout << "(no_abort)";
          break;
        case cmdlineparse::abort_reason::error_extra_arg:
          std::cout << "error_extra_arg";
          break;
        case cmdlineparse::abort_reason::error_no_arg:
          std::cout << "error_no_arg";
          break;
        case cmdlineparse::abort_reason::error_ambiguous_option:
          std::cout << "error_ambiguous_option";
          break;
        case cmdlineparse::abort_reason::error_unknown_option:
          std::cout << "error_unknown_option";
          break;
        case cmdlineparse::abort_reason::error_no_arg_short:
          std::cout << "error_no_arg_short";
          break;
        case cmdlineparse::abort_reason::error_unknown_option_short:
          std::cout << "error_unknown_option_short";
          break;
        default:
          std::cout << "UNKNOWN";
          break;
        }
      std::cout << " -- " << cmd.get_abort_option () << std::endl;
      return 1;
    }

  // Output version
  std::cout << cmd.get_version_string () << std::endl;

  // Output results
  std::cout << "Parse result:" << std::endl;
  std::cout << "  string option = \"" << str << "\"" << std::endl;
  std::cout << "  string option 2 = \"" << str2 << "\"" << std::endl;
  std::cout << "  flag = " << ( flag ? "true" : "false" ) << std::endl;
  std::cout << "  flag 2 = " << ( flag2 ? "true" : "false" ) << std::endl;

  // Output unamed arguments
  std::cout << "Unamed arguments:" << std::endl;
  for (auto x : cmd.get_unamed_args ())
    {
      std::cout << "  unamed arg = \"" << x << "\"" << std::endl;
    }

  return 0;
}

namespace
{
  bool callback_function (const std::string &optarg)
  {
    std::cout << "callback function \"" << optarg << "\"" << std::endl;
    return true;
  }
}
