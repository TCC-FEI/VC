AM_CPPFLAGS = $(DEPS_CFLAGS)

bin_PROGRAMS = voice-control

voice_control_SOURCES = main.c \
                        vc/console/console.c \
                        vc/console/commands.c \
                        vc/control/manager.c \
                        vc/control/handler.c \
                        vc/vc.h \
                        vc/app.h \
                        vc/console/types.h \
                        vc/console/console.h \
                        vc/console/commands.h \
                        vc/control/types.h \
                        vc/control/plugin.h \
                        vc/control/handler.h \
                        vc/control/manager.h

if DEBUG
    voice_control_SOURCES += utils.h utils.c
endif

voice_control_LDADD = $(DEPS_LIBS)


lib_LTLIBRARIES = libvcc-plugin-@VCC_PLUGIN_API_VERSION@.la
libvcc_plugin_@VCC_PLUGIN_API_VERSION@_la_SOURCES = vc/control/plugin.h vc/control/plugin.c
libvcc_plugin_@VCC_PLUGIN_API_VERSION@_la_LDFLAGS = -version-info $(VCC_PLUGIN_SO_VERSION)
libvcc_plugin_@VCC_PLUGIN_API_VERSION@_la_LIBADD = $(DEPS_LIBS)
vcc_plugin_includedir = $(includedir)/vcc-plugin-$(VCC_PLUGIN_API_VERSION)
nobase_vcc_plugin_include_HEADERS = vc/control/plugin.h

vcc_plugin_libincludedir = $(libdir)/vcc-plugin-$(VCC_PLUGIN_API_VERSION)/include
nodist_vcc_plugin_libinclude_HEADERS = ../config.h

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = ../vcc-plugin-$(VCC_PLUGIN_API_VERSION).pc

pkglibdir = $(libdir)/plugins
pkglib_LTLIBRARIES = libgedit-@VCC_PLUGIN_API_VERSION@.la
libgedit_@VCC_PLUGIN_API_VERSION@_la_SOURCES = vc/control/plugin.h plugins/control/gedit/gedit.c
libgedit_@VCC_PLUGIN_API_VERSION@_la_LDFLAGS = -version-info $(VCC_PLUGIN_SO_VERSION)
libgedit_@VCC_PLUGIN_API_VERSION@_la_LIBADD = $(DEPS_LIBS) libvcc-plugin-$(VCC_PLUGIN_API_VERSION).la
