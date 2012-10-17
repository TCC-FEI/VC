AM_CPPFLAGS = $(DEPS_CFLAGS)

bin_PROGRAMS = voice-control

voice_control_SOURCES = main.c \
                        vc/console.c \
                        vc/console_commands.c \
                        vc/control_manager.c \
                        vc/control_handler.c \
                        vc/vc.h \
                        vc/app.h \
                        vc/console_types.h \
                        vc/console.h \
                        vc/console_commands.h \
                        vc/control_types.h \
                        vc/control_plugin.h \
                        vc/control_handler.h \
                        vc/control_manager.h

if DEBUG
    voice_control_SOURCES += utils.h utils.c
endif

voice_control_LDADD = $(DEPS_LIBS)
