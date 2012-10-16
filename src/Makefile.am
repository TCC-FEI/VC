AM_CPPFLAGS = $(DEPS_CFLAGS)

bin_PROGRAMS = voice-control

voice_control_SOURCES = main.c \
                        console/console.c \
                        console/commands.c \
                        control/manager.c \
                        control/handler.c \
                        vc.h \
                        app.h \
                        console/types.h \
                        console/console.h \
                        console/commands.h \
                        control/types.h \
                        control/plugin.h \
                        control/handler.h \
                        control/manager.h

if DEBUG
    voice_control_SOURCES += utils.h utils.c
endif

voice_control_LDADD = $(DEPS_LIBS)


MAINTAINERCLEANFILES = config.h.in Makefile.in