AC_INIT([voice-control], [1.0.0], [blabos@gmail.com])

AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([foreign -Wall -Werror])

AC_PROG_CC

AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile src/Makefile])

AC_DEFINE([WNCK_I_KNOW_THIS_IS_UNSTABLE], 1,
          [alerts libwnck that I know what I am doing])

PKG_CHECK_MODULES([DEPS], [glib-2.0 libwnck-3.0 x11])


AC_ARG_ENABLE([debug],
              [AS_HELP_STRING([--enable-debug], [activate debug messages])],
              [case "${enableval}" in
                no) debug=false ;;
                *)  debug=true;
                    AC_DEFINE([DEBUG], [], [Activate debug messages]) ;;
              esac],
              [debug=false])

AM_CONDITIONAL([DEBUG], [test x$debug = xtrue])


AC_OUTPUT
