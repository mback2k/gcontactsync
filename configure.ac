#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.67])
AC_INIT([gcontactsync], [0.9], [info@marc-hoersken.de])

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_SRCDIR([src/gcontactsync.c])
AC_CONFIG_AUX_DIR([scripts])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([foreign -Wall -Werror])

AC_LIBTOOL_WIN32_DLL

m4_ifdef([AM_PROG_AR], [AM_PROG_AR])
LT_INIT
LT_PROG_RC
AM_CONDITIONAL([RC], test $RC)

dnl Checks for programs.
AC_PROG_MAKE_SET
AC_PROG_INSTALL
AC_PROG_LIBTOOL
AC_PROG_CC

dnl Checks for libraries.
PKG_CHECK_MODULES([GLIB], [glib-2.0],, \
	AC_MSG_ERROR([*** glib2 not found! You need it to build $PACKAGE_NAME. ***]))
AC_SUBST(GLIB_CFLAGS)
AC_SUBST(GLIB_LIBS)

PKG_CHECK_MODULES([PURPLE], [purple],, \
	AC_MSG_ERROR([*** purple not found! You need it to build $PACKAGE_NAME. ***]))
AC_SUBST(PURPLE_CFLAGS)
AC_SUBST(PURPLE_LIBS)

PKG_CHECK_MODULES([LIBXML], [libxml-2.0],, \
	AC_MSG_ERROR([*** libxml2 not found! You need it to build $PACKAGE_NAME. ***]))
AC_SUBST(LIBXML_CFLAGS)
AC_SUBST(LIBXML_LIBS)

PKG_CHECK_MODULES([LIBCURL], [libcurl],, \
	AC_MSG_ERROR([*** libcurl not found! You need it to build $PACKAGE_NAME. ***]))
AC_SUBST(LIBCURL_CFLAGS)
AC_SUBST(LIBCURL_LIBS)

PKG_CHECK_MODULES([LIBGCAL], [libgcal],, \
	AC_MSG_ERROR([*** libgcal not found! You need it to build $PACKAGE_NAME. ***]))
AC_SUBST(LIBGCAL_CFLAGS)
AC_SUBST(LIBGCAL_LIBS)

dnl Checks for header files.
AC_CHECK_HEADERS([string.h])

dnl Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

dnl Checks for library functions.
AC_CHECK_FUNCS([strdup strndup strstr strlen])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
