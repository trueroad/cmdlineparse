#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([One header file Commandline Parse for C++11],
[2019-11-17.07], , [cmdlineparse],
[https://github.com/trueroad/cmdlineparse])
AM_INIT_AUTOMAKE([foreign])
AM_GNU_GETTEXT_VERSION([0.18.2])
AM_GNU_GETTEXT([external])
AC_CONFIG_SRCDIR([advanced-sample.cc])
AC_CONFIG_HEADERS([config.h])

PACKAGE_COPYRIGHT="Copyright (C) 2016-2019 Masamichi Hosoda."
PACKAGE_COPYRIGHT="$PACKAGE_COPYRIGHT All rights reserved."
PACKAGE_LICENSE="License: BSD-2-Clause"

AC_SUBST([PACKAGE_COPYRIGHT])
AC_SUBST([PACKAGE_LICENSE])
AC_DEFINE_UNQUOTED([PACKAGE_COPYRIGHT], ["$PACKAGE_COPYRIGHT"],
[Define to the copyright of this package.])
AC_DEFINE_UNQUOTED([PACKAGE_LICENSE], ["$PACKAGE_LICENSE"],
[Define to the license of this package.])

# Checks for programs.
AC_PROG_CXX
AX_CXX_COMPILE_STDCXX_11([noext], [mandatory])

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile po/Makefile.in])
AC_OUTPUT
