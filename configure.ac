# Copyright 2021-2022 MarcosHCK
# This file is part of lglm.
#
# lglm is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# lglm is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with lglm.  If not, see <http://www.gnu.org/licenses/>.
#
AC_PREREQ([2.69])
m4_define([lglm_VERSION], m4_esyscmd([printf `sed -e 's/^#define VERSION "\([^"]*\)"/\1/p' -e d < ./version.h`]))
AC_INIT([lglm], [lglm_VERSION], [lglm.bugs@hcktm.com], [lglm], [https://www.hcktm.com/lglm/])
AC_CONFIG_SRCDIR([src/lglm.c])
AC_LANG([C])

AC_CONFIG_MACRO_DIRS([aclocal])

CFLAGS="-Isrc/ $CFLAGS"
m4_define([lglm_RELEASE], m4_esyscmd([printf `sed -e 's/^#define RELEASE "\([^"]*\)"/\1/p' -e d < ./version.h`]))
AC_SUBST([PACKAGE_RELEASE], [lglm_RELEASE])

# Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_MAKE_SET
LT_INIT
PKG_PROG_PKG_CONFIG

AC_SUBST([SHARED_FLAGS])

case "${host_os}" in
  darwin*)
    SHARED_FLAGS=-dynamiclib
    ;;
  mingw*)
    SHARED_FLAGS=-shared
    ;;
  cygwin*)
    SHARED_FLAGS=-shared
    ;;
  *)
    SHARED_FLAGS=-shared
    ;;
esac

if test SHARED_FLAGS == ""; then
  AC_MSG_ERROR([Can not guess shared configuration for your system])
fi

debugging="no"

AC_ARG_ENABLE(debug,
              AS_HELP_STRING([--enable-debug-output], [Enable debug output @<:@no@:>@]),
              [if test "x$enableval" != "xno"; then
                debugging="yes"
               fi
              ])

if test "x$debugging" == "xno"; then
  CFLAGS="-O2 $CFLAGS"
  LDFLAGS="-s $LDFLAGS"
else
  CFLAGS="-g $CFLAGS"
fi

# Checks for libraries.
AC_SEARCH_LIBS([pow], [m])

# Check for libraries using pkg-config
#PKG_CHECK_MODULES([LUA], [luajit-2.0 lua5.3 lua5.2 lua5.1])
PKG_CHECK_MODULES([LUA], [luajit-2.0],,[
  PKG_CHECK_MODULES([LUA], [lua5.3],, [
    PKG_CHECK_MODULES([LUA], [lua5.2],, [
      PKG_CHECK_MODULES([LUA], [lua5.1])
    ])
  ])
])
PKG_CHECK_MODULES([CGLM], [cglm])

# Checks for header files.
AC_CHECK_HEADERS([stddef.h stdint.h stdlib.h string.h math.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE
AC_TYPE_SIZE_T
AC_TYPE_UINT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T

# Checks for library functions.
AC_CHECK_FUNCS([memset])
AC_CHECK_FUNCS([memcpy])

# Produce output
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
