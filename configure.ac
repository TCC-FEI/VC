AC_INIT([voice-control], [1.0.0], [blabos@gmail.com])
AC_PREREQ([2.68])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([1.11 foreign -Wall -Werror])

LT_INIT([disable-static])

AC_PROG_CC

AC_SUBST([VCC_PLUGIN_VERSION],     [1.0.0])
AC_SUBST([VCC_PLUGIN_SO_VERSION],  [1:0:0])
AC_SUBST([VCC_PLUGIN_API_VERSION], [1.0])

AC_SUBST([VCS_PLUGIN_VERSION],     [1.0.0])
AC_SUBST([VCS_PLUGIN_SO_VERSION],  [1:0:0])
AC_SUBST([VCS_PLUGIN_API_VERSION], [1.0])

AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
    Makefile
    src/Makefile
    vcc-plugin-${VCC_PLUGIN_API_VERSION}.pc:vcc-plugin.pc.in
])

AC_DEFINE_UNQUOTED(
    [VCC_PLUGIN_API_VERSION],
    ["${VCC_PLUGIN_API_VERSION}"],
    [foobar]
)

AC_DEFINE_UNQUOTED([PREFIX], ["${prefix}"], [get prefix])
AC_DEFINE_UNQUOTED(
    [VCS_DATA_DIR],
    ["${prefix}/share/${PACKAGE_NAME}"],
    [get datadir]
)

AC_DEFINE([WNCK_I_KNOW_THIS_IS_UNSTABLE], 1,
          [alerts libwnck that I know what I am doing])

PKG_CHECK_MODULES([APP_DEPS], [glib-2.0 libwnck-3.0 x11])
PKG_CHECK_MODULES([VCC_DEPS], [glib-2.0 libwnck-3.0 x11 xtst])
PKG_CHECK_MODULES([VCS_DEPS], [glib-2.0 gstreamer-0.10])


AC_ARG_ENABLE([debug],
              [AS_HELP_STRING([--enable-debug], [activate debug messages])],
              [case "${enableval}" in
                no) debug=false ;;
                *)  debug=true;
                    AC_DEFINE([DEBUG], [], [Activate debug messages]) ;;
              esac],
              [debug=false])

AM_CONDITIONAL([DEBUG], [test x$debug = xtrue])
AM_PROG_CC_C_O


AC_OUTPUT
