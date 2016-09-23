<!-- -*- coding: utf-8 -*- -->
# One header file *Commandline Parse* for C++11

*Commandline Parse* C++11 用のコマンドラインパーサ
（コマンドラインオプション解析器）です。
ヘッダファイル一つだけでできていますので、簡単に使えます。

## サンプルをビルドする

*Commandline Parse* そのものは Autotools 不要ですが、
サンプルのビルドには Autotools が必要になります。

```
$ ./autogen.sh
$ ./configure
$ make
```

## 使い方

### シンプルなサンプル

以下は [simple-sample.cc](./simple-sample.cc) からの抜粋です。

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

以下、いくつかの出力例です。

短いオプション `-h` は help を表示して終了します。

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

何もオプションを付けないとデフォルトになります。

```
$ ./simple-sample
string option = "foobar"
flag = false

```

文字列 (string) オプションを使うと、指定した変数に値が設定されます。
`-s baz`, `-sbaz`, `--string baz`, `--string=baz`
のいずれのスタイルでも同じ結果となります。

```
$ ./simple-sample -s baz
string option = "baz"
flag = false

```

フラグ (flag) オプションを使うと、指定したフラグが true になります。
`-f`, `--flag` いずれのスタイルでも同じ結果となります。

```
$ ./simple-sample -f
string option = "foobar"
flag = true

```

以下、複数のオプションを組み合わせた場合の例です。

```
$ ./simple-sample --string=baz -f
string option = "baz"
flag = true

```

以下、もう一つの組み合わせの例です。

```
$ ./simple-sample -fsbaz
string option = "baz"
flag = true

```

文字列 (string) オプションには引数が必要です。

```
$ ./simple-sample -s
./simple-sample: option requires an argument -- s

```

フラグ (flag) オプションは引数を使えません。

```
$ ./simple-sample --flag=baz
./simple-sample: option doesn't take an argument -- flag

```

一部省略した長いオプションが使えます。

```
$ ./simple-sample --fl --str=baz
string option = "baz"
flag = true

```

### 高度なサンプル

高度なサンプルは [advanced-sample.cc](./advanced-sample.cc) にあります。
以下、出力の例です。

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

## 定義

### オプションを追加する

#### オプションハンドラの追加

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

オプションハンドラを追加します。

* パラメータ:
  + `short_name`: 短いオプション。0 の場合は短いオプション無しとなります。
  + `long_name`: 長いオプション。空の場合は長いオプション無しとなります。
  + `has_arg`: `getopt_long ()` と同様の下記 3 種類のいずれか。
    - `no_argument`: オプションは引数を取らない。
    - `required_argument`: オプションは引数が必須。
    - `optional_argument`: オプションは引数があってもなくてもよい。
  + `option_handler`: オプションハンドラ。
    パーサがオプションを見つけたら呼び出されます。
    ハンドラのパラメータはオプションの引数または空です。
    ハンドラが false を返した場合、パースは中断されます。
  + `description`: オプションの説明本文。
    help メッセージに追加されます。
    UTF-8 等のマルチバイト文字列の場合に困難となるため、
    自動改行や自動インデントされません。
  + `typestr`: 引数の type string。
    help メッセージに追加されます。
  + `header`: オプションの説明ヘッダ。
    help メッセージに追加されます。
  + `group`: オプショングループの名前。
    help メッセージのグループに使われます。
* 返り値:
  + `true`: 追加完了。
  + `false`: オプション名は既に登録されています。なにも変更しませんでした。

#### フラグ (flag) オプションの追加

```c++
bool add_flag (char short_name,
               const std::string &long_name,
               bool *flag,
               const std::string &description = "",
               const std::string &group = "");
```

フラグ (flag) オプションを追加します。

* パラメータ:
  + `flag`: フラグへのポインタ。以下のいずれかがセットされます。
    + `true`: このオプションが指定されました。
    + `false`: このオプションは指定されませんでした。
  + 他のパラメータは `add_handler ()` と同じです。
* 返り値:
  + 返り値は `add_handler ()` と同じです。

#### 文字列 (string) オプションの追加

```c++
bool add_string (char short_name,
                 const std::string &long_name,
                 std::string *var,
                 const std::string &defval,
                 const std::string &description = "",
                 const std::string &typestr = "STRING",
                 const std::string &group = "");
```

文字列 (string) オプションを追加します。

* パラメータ:
  + `var`: 文字列へのポインタ。
    このオプションが指定された時には、その引数が `var` にセットされます。
  + `defval`: デフォルト値。
    このオプションが指定されなかった時には、この値が `var` にセットされます。
  + 他のパラメータは `add_handler ()` と同じです。
* 返り値:
  + 返り値は `add_handler ()` と同じです。

#### help 説明文の追加

```c++
void add_description (char short_name,
                      const std::string &long_name,
                      arg_mode has_arg,
                      const std::string &description,
                      const std::string &typestr = "",
                      const std::string &header = "",
                      const std::string &group = "");
```

help 説明文を追加します。
説明文の追加のみです。
パーサの動作は変更されません。

* パラメータ:
  + 全てのパラメータは `add_handler ()` と同じです。

#### デフォルトハンドラの追加

##### help

```c++
bool add_default_help (void);
```

デフォルトの help オプションハンドラを追加します。
`-h`, `--help` オプションが追加されます。

* 返り値:
  + 返り値は `add_handler ()` と同じです。

##### version

```c++
bool add_default_version (void);
```

デフォルトの version オプションハンドラを追加します。
`-V`, `--version` オプションが追加されます。

* 返り値:
  + 返り値は `add_handler ()` と同じです。

##### help と version

```c++
bool add_default (void);
```

デフォルトのオプションハンドラを追加します。
`-h`, `--help`, `-V`, `--version` オプションが追加されます。

* 返り値:
  + 返り値は `add_handler ()` と同じです。

### パース

#### オプションをパース

```c++
bool parse (int argc, char const* const* argv, int optind = 1);
```

オプションをパースします。

* パラメータ:
  + `argc`, `argv`: `main ()` の引数。
  + `optind`: パースする最初の引数のインデックス。
* 返り値:
  + `true`: パース成功。
    `get_unamed_args ()` が使えるようになります。
  + `false`: パース中断。
    `get_abort_reason ()` と `get_abort_option ()` が使えるようになります。

#### unamed_args 取得

```c++
const std::vector<std::string> &get_unamed_args () const noexcept;
```

無名の引数（追加のオプション）を取得します。
この関数はパースが成功した後で使うことができます。

#### 中断理由の取得

```c++
abort_reason get_abort_reason () const noexcept;
```

中断理由を取得します。
この関数はパースが中断した後で使うことができます。

#### 中断したときのオプションを取得

```c++
const std::string &get_abort_option () const noexcept;
```

中断したときのオプションを取得します。
この関数はパースが中断した後で使うことができます。

### 文字列の構築／取得

#### デフォルト文字列の取得

##### 使い方 (usage)

```c++
std::string build_usage (void) const;
```

使い方 (usage) 文字列を構築します。
カスタムのヘルプハンドラ等に使うことができます。

##### help

```c++
std::string build_usage (void) const;
```

help 文字列を構築します。
カスタムのヘルプハンドラ等に使うことができます。

##### version

```c++
const std::string &get_version_string () const noexcept;
```

バージョン文字列を取得します。
カスタムのバージョンハンドラ等に使うことができます。

#### 定数

##### ヘッダのデフォルトインデント

```c++
const std::string h_indent;
```

説明ヘッダのデフォルトインデントです。
`add_* ()` の `header` 引数等に使用できます。

##### ヘッダのデフォルトセパレータスペース

```c++
const std::string h_space;
```

説明ヘッダのデフォルトセパレータスペースです。
`add_* ()` の `header` 引数等に使用できます。

##### 説明本文のデフォルトインデント

```c++
const std::string d_indent;
```

説明本文のデフォルトインデントです。
`add_* ()` の `description` 引数等に使用できます。

### カスタマイズ

#### デフォルト文字列の変更

##### version 文字列

```c++
void set_version_string (const std::string &s);
```

バージョン文字列を設定します。
`get_version_string ()`, `build_help ()`
とデフォルトのオプションハンドラがこの文字列を使います。

この関数を使わなかった場合は、デフォルトの文字列が使われます。
Autotools を使っている場合は、マクロでデフォルト文字列が設定されます。
Autotools を使っていない場合は、`argv[0]` が使われます。

##### usage_unamed_opts 文字列

```c++
void set_usage_unamed_opts (const std::string &s);
```

usage_unamed_opts 文字列を設定します。
`build_usage ()`, `build_help ()`
とデフォルトのヘルプハンドラがこの文字列を使います。

#### デフォルトの動作を変更

##### opterr

```c++
void set_opterr (bool flag) noexcept;
```

+ `true` (default):
  パーサはエラーが発生したら標準出力へエラーメッセージを出力します。
+ `false`:
  パーサはエラーが発生しても何も出力しません。

##### long_only

```c++
void set_long_only (bool flag) noexcept;
```

+ `true`: `getopt_long_only ()` のような動作になります。
  つまり、ハイフン一つの長い名前のオプション（たとえば -long_option）
  が使えるようになります。
+ `false` (default): `getopt_long ()` のような動作になります。
  つまり、長いオプションはハイフン二つが必須（たとえば --long_option）
  となります。

##### continue_on_error

```c++
void set_continue_on_error (bool flag) noexcept;
```

+ `true`: エラーが発生してもパースを続けます。
+ `false` (default): エラーが発生したらパースを中断します。

##### abbreviated_long_name

```c++
void set_abbreviated_long_name (bool flag) noexcept;
```

+ `true` (default): 一部省略した長いオプションが使えます。
+ `false`: 長いオプションは完全一致する必要があります。

## License

Copyright (C) 2016 Masamichi Hosoda. All rights reserved.

License: BSD-2-Clause

[LICENSE](./LICENSE) をご覧ください。

もし GPL が良い場合には、
[Extract PDFmark](https://github.com/trueroad/extractpdfmark)
に GPL 版の *Commandline Parse* があります。