# One header file *Commandline Parse* for C++11

*Commandline Parse* is a command line parser for C++11.
It consists of only one header file.
Therefore, you can use it easily.

## Build samples

*Commandline Parse* does not require Autotools.
But, these samples use Autotools.

```
$ ./autogen.sh
$ ./configure
$ make
```

## Usage

### Simple sample

Here is a part of `simple-sample.cc`.

```c++
#include <iostream>
#include "cmdlineparse.hh"

int main (int argc, char *argv[])
{
  // Prepare parser instance
  cmdlineparse::parser cmd;

  // Prepare variable to store the parse result
  std::string str;
  bool flag;

  // Add `help' and `version' options
  cmd.add_default ();

  // Add a string option
  //   1st arg: short option name
  //   2nd arg: long option name
  //   3rd arg: a pointer to store the parse result
  //   4th arg: default value
  //   5th arg: description for `help'
  //            `d_indent' is description default indent.
  cmd.add_string ('s', "string", &str, "foobar",
                  cmd.d_indent + "Specify string option.");

  // Add a flag option
  //   1st arg: short option name
  //   2nd arg: long option name
  //   3rd arg: pointer to store the parse result
  //   4th arg: description for `help'
  //            `d_indent' is description default indent.
  cmd.add_flag ('f', "flag", &flag,
                cmd.d_indent + "This is a flag.\n" +
                cmd.d_indent + "Second line.\n" +
                cmd.d_indent + "This parser doesn't warp text automatically.");

  // Parse
  if (!cmd.parse (argc, argv))
    return 0;

  // Output results
  std::cout << "string option = \"" << str << "\"" << std::endl;
  std::cout << "flag = " << ( flag ? "true" : "false" ) << std::endl;

  return 0;
}
```

Here are some outputs:

Short option `-h` prints help and exit.

```
$ ./simple-sample -h
One header file Commandline Parse for C++11 2016-09-19.22
Copyright (C) 2016 Masamichi Hosoda. All rights reserved.
License: BSD-2-Clause

https://github.com/trueroad/cmdlineparse/

Usage: ./simple-sample [options] ...

  -h, --help
    Print help and exit
  -V, --version
    Print version and exit
  -s, --string=STRING   (default=foobar)
    Specify string option.
  -f, --flag
    This is a flag.
    Second line.
    This parser doesn't warp text automatically.

```

No options produces default results.

```
$ ./simple-sample
string option = "foobar"
flag = false

```

String option sets a variable.
All option style `-s baz`, `-sbaz`, `--string baz`, and `--string=baz`
provide same result.

```
$ ./simple-sample -s baz
string option = "baz"
flag = false

```

Flag option sets true.
All option style `-f` and `--flag` provide same result.

```
$ ./simple-sample -f
string option = "foobar"
flag = true

```

Here is combined example.

```
$ ./simple-sample --string=baz -f
string option = "baz"
flag = true

```

Here is another combined example.

```
$ ./simple-sample -fsbaz
string option = "baz"
flag = true

```

String option requires an argument.

```
$ ./simple-sample -s
./simple-sample: option requires an argument -- s

```

Flag option cannot have arguments.

```
$ ./simple-sample --flag=baz
./simple-sample: option doesn't take an argument -- flag

```

Abbreviated long option can be used.

```
$ ./simple-sample --fl --str=baz
string option = "baz"
flag = true

```

### Advanced sample

Advanced sample is `advanced-sample.cc`.
Here is an output:

```
$ ./advanced-sample --help
One header file Commandline Parse for C++11: Advanced Sample
Copyright (C) 2016 Masamichi Hosoda. All rights reserved.

Usage: ./advanced-sample [options] [FILES] ...

  -h, --help
    Print help and exit
  -V, --version
    Print version and exit
  -s, --string=STRING   (default=foo)
    Specify string option.
  -f, --flag
    This is a flag.
    Second line.
    This parser doesn't warp text automatically.
  -H, --handler[=OPTION]
    This is option handler
Description only
  description only

Group1:
  -tFILENAME   (default=bar)
    Specify string option 2.
  --flag-another
    This is a flag 2.
  -F, --callback-func=OPTARG   (OPTARG is required)
    This is callback hander
  Description 2
    description only 2 - first line
    description only 2 - second line

Group2:
  -a, --abort=FLAG   (possible value=true, false)
    This option aborts parsing
    If FLAG=true, abort parsing
Abort reason is option_handler -- help

```

## Declaration

### Add option

#### Add an option handler

```c++
bool add_handler (char short_name,
                  const std::string &long_name,
                  arg_mode has_arg,
                  std::function<bool(const std::string&)> option_handler,
                  const std::string &description = "",
                  const std::string &typestr = "",
                  const std::string &header = "",
                  const std::string &group = "");
```

Add an option handler.

* Parameters:
  + `short_name`: Short option name. If 0, `short_name` is not added.
  + `long_name`: Long option name. If empty, `long_name` is not added.
  + `has_arg`: One of the followings three like `getopt_long ()`.
    - `no_argument`: The option does not take an argument.
    - `required_argument`: The option requires an argument.
    - `optional_argument`: The option takes an argument.
  + `option_handler`: Option handler.
    It is called when the parser found the option.
    Its parameter is the option's argument or empty.
    If it returns false, the parsing is aborted.
  + `description`: Description body of the option.
    It is added to a help message.
	It doesn't warp text automatically
	because wrapping is difficult for multibyte encodings like UTF-8 etc.
  + `typestr`: Argument type string.
    It is added to a help message.
  + `header`: Description header of the option.
    It is added to a help message.
  + `group`: Group name of the option.
    It is used for help message group.
* Return value:
  + `true`: The addition has been completed.
  + `false`: The given option name already has been registered.
    Nothing was changed.

#### Add a flag option

```c++
bool add_flag (char short_name,
               const std::string &long_name,
               bool *flag,
               const std::string &description = "",
               const std::string &group = "");
```

Add a flag option.

* Parameter
  + `flag`: A pointer of flag. One of the following will be set.
    + `true`: The parser found the option.
	+ `false`: The parser didn't found the option.
  + Other parameters are same as `add_handler ()`.
* Return value:
  + Return value is same as `add_handler ()`.

#### Add a string option

```c++
bool add_string (char short_name,
                 const std::string &long_name,
                 std::string *var,
				 const std::string &defval,
                 const std::string &description = "",
                 const std::string &typestr = "STRING",
                 const std::string &group = "");
```

Add a string option.

* Parameter
  + `var`: A pointer of string.
    When the parser found the option, its argument is set to `var`.
  + `defval`: Default value.
    When the parser didn't fount the option, this value is set to `var`.
  + Other parameters are same as `add_handler ()`.
* Return value:
  + Return value is same as `add_handler ()`.

#### Add help description

```c++
void add_description (char short_name,
                      const std::string &long_name,
                      arg_mode has_arg,
                      const std::string &description,
                      const std::string &typestr = "",
                      const std::string &header = "",
                      const std::string &group = "");
```

Add help description.
This function adds only the description.
It is added to a help message.
The parser behavior is not changed.

* Parameter
  + All parameters are same as `add_handler ()`.

#### Add default handler

##### help

```c++
bool add_default_help (void);
```

Add default help option handler.
This function adds `-h` and `--help` option.

* Return value:
  + Return value is same as `add_handler ()`.

##### version

```c++
bool add_default_version (void);
```

Add default version option handler.
This function adds `-V` and `--version` option.

* Return value:
  + Return value is same as `add_handler ()`.

##### both help and version

```c++
bool add_default (void);
```

Add default option handler.
This function adds `-h`, `--help`, `-V`, and `--version` options.

* Return value:
  + Return value is same as `add_handler ()`.

### Parse

#### Parse options

```c++
bool parse (int argc, char const* const* argv, int optind = 1);
```

Parse options.

* Parameter
  + `argc` and `argv`: Arguments from `main ()`.
  + `optind`: The index of the first argument for parsing.
* Return value:
  + `true`: The parsing has been completed.
    `get_unamed_args ()` can be used.
  + `false`: The parsing was aborted.
    `get_abort_reason ()` and `get_abort_option ()` can be used.

#### Get unamed_args

```c++
const std::vector<std::string> &get_unamed_args () const noexcept;
```

Get unnamed arguments (extra options).
This function can be used after the parsing has been completed.

#### Get abort reason

```c++
abort_reason get_abort_reason () const noexcept;
```

Get abort reason.
This function can be used after the parsing was aborted.

#### Get abort option

```c++
const std::string &get_abort_option () const noexcept;
```

Get abort option.
This function can be used after the parsing was aborted.

### Build / Get strings

#### Build default strings

##### usage

```c++
std::string build_usage (void) const;
```

Build usage strings. It can be used for custom help handler etc.

##### help

```c++
std::string build_usage (void) const;
```

Build help strings. It can be used for custom help handler etc.

##### version

```c++
const std::string &get_version_string () const noexcept;
```

Get version strings. It can be used for custom version handler etc.

#### Constant

##### header default indent

```c++
const std::string h_indent;
```

Description header default indent.
It can be used for `header` argumnent of `add_* ()` etc.

##### header default separator spaces

```c++
const std::string h_space;
```

Description header default separator spaces.
It can be used for `header` argumnent of `add_* ()` etc.

##### description default indent

```c++
const std::string d_indent;
```

Description body default indent.
It can be used for `description` argumnent of `add_* ()` etc.

### Customize

#### Change default strings

##### version strings

```c++
void set_version_string (const std::string &s);
```

Set version strings.
`get_version_string ()`, `build_help ()`,
and default option handlers use this strings.

If you doesn't use this function,
default version strings is used.
If you use Autotools,
the default version strings is built from Autotools macro.
If you don't use Autotools,
the default version strings is built from `argv[0]`.

##### usage_unamed_opts strings

```c++
void set_usage_unamed_opts (const std::string &s);
```

Set usage_unamed_opts strings.
`build_usage ()`, `build_help ()`,
and default help handlers use this strings.

#### Change default behavior

##### opterr

```c++
void set_opterr (bool flag) noexcept;
```

+ `true` (default):
  The parser outputs an error message to standard output
  when the error happens.
+ `false`:
  The parser doesn't output any error messages.

##### long_only

```c++
void set_long_only (bool flag) noexcept;
```

+ `true`: The parser behavior becomes like `getopt_long_only ()`.
  i.e. single hyphen long option (something like -long_option) is acceptable.
+ `false` (default): The parser behavior becomes like `getopt_long ()`.
  i.e. long option must have double hyphen (something like --long_option).

##### continue_on_error

```c++
void set_continue_on_error (bool flag) noexcept;
```

+ `true`: The parser continues parsing even if an error happened.
+ `false` (default): The parser aborts parsing when an error happened.

##### abbreviated_long_name

```c++
void set_abbreviated_long_name (bool flag) noexcept;
```

+ `true` (default): Abbreviated long option can be used.
+ `false`: Long option must be an exact match.

## License

Copyright (C) 2016 Masamichi Hosoda. All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY AUTHOR AND CONTRIBUTORS ``AS IS'' AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED.  IN NO EVENT SHALL AUTHOR OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
SUCH DAMAGE.
